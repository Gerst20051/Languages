#include "node.h"
#include "platform.h"

#include <v8.h>

#include <sys/param.h> // for MAXPATHLEN
#include <sys/sysinfo.h>
#include <unistd.h> // getpagesize, sysconf
#include <stdio.h> // sscanf, snprintf
#include <string.h>

#include <windows.h>


namespace node {

using namespace v8;

static char buf[MAXPATHLEN + 1];
static char *process_title = NULL;


// Does the about the same as perror(), but for windows api functions
static void _winapi_perror(const char* prefix = NULL) {
  DWORD errorno = GetLastError();
  char *errmsg;

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorno, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errmsg, 0, NULL);

  // FormatMessage messages include a newline character

  if (prefix) {
    fprintf(stderr, "%s: %s", prefix, errmsg);
  } else {
    fputs(errmsg, stderr);
  }
}


char** Platform::SetupArgs(int argc, char *argv[]) {
  return argv;
}


void Platform::SetProcessTitle(char *title) {
  // We need to convert _title_ to UTF-16 first, because that's what windows uses internally.
  // It would be more efficient to use the UTF-16 value that we can obtain from v8,
  // but it's not accessible from here.

  // Max title length; according to the specs it should be 64K but in practice it's a little over 30000,
  // but who needs titles that long anyway?
  const int MAX_TITLE_LENGTH = 30001;

  int length;
  WCHAR *title_w;

  // Find out how big the buffer for the wide-char title must be
  length = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);
  if (!length) {
    _winapi_perror("MultiByteToWideChar");
    return;
  }

  // Convert to wide-char string
  title_w = new WCHAR[length];
  length = MultiByteToWideChar(CP_UTF8, 0, title, -1, title_w, length);
  if (!length) {
    _winapi_perror("MultiByteToWideChar");
    delete title_w;
    return;
  };

  // If the title must be truncated insert a \0 terminator there
  if (length > MAX_TITLE_LENGTH) {
    title_w[MAX_TITLE_LENGTH - 1] = *L"\0";
  }

  if (!SetConsoleTitleW(title_w)) {
    _winapi_perror("SetConsoleTitleW");
  }

  free(process_title);
  process_title = strdup(title);

  delete title_w;
}


static inline char* _getProcessTitle() {
  WCHAR *title_w;
  char *title;
  int length, length_w;

  length_w = GetConsoleTitleW((WCHAR*)L"\0", sizeof(WCHAR));

  // If length is zero, there may be an error or the title may be empty
  if (!length_w) {
    if (GetLastError()) {
      _winapi_perror("GetConsoleTitleW");
      return NULL;
    }
    else {
      // The title is empty, so return empty string
      process_title = strdup("\0");
      return process_title;
    }
  }

  // Room for \0 terminator
  length_w++;

  title_w = new WCHAR[length_w];

  if (!GetConsoleTitleW(title_w, length_w * sizeof(WCHAR))) {
    _winapi_perror("GetConsoleTitleW");
    delete title_w;
    return NULL;
  }

  // Find out what the size of the buffer is that we need
  length = WideCharToMultiByte(CP_UTF8, 0, title_w, length_w, NULL, 0, NULL, NULL);
  if (!length) {
    _winapi_perror("WideCharToMultiByte");
    delete title_w;
    return NULL;
  }

  title = (char *) malloc(length);
  if (!title) {
    perror("malloc");
    delete title_w;
    return NULL;
  }

  // Do utf16 -> utf8 conversion here
  if (!WideCharToMultiByte(CP_UTF8, 0, title_w, -1, title, length, NULL, NULL)) {
    _winapi_perror("WideCharToMultiByte");
    delete title_w;
    free(title);
    return NULL;
  }

  delete title_w;

  return title;
}


const char* Platform::GetProcessTitle(int *len) {
  // If the process_title was never read before nor explicitly set,
  // we must query it with getConsoleTitleW
  if (!process_title) {
    process_title = _getProcessTitle();
  }

  if (process_title) {
    *len = strlen(process_title);
    return process_title;
  } else {
    *len = 0;
    return NULL;
  }
}


int Platform::GetMemory(size_t *rss, size_t *vsize) {
  FILE *f = fopen("/proc/self/stat", "r");
  if (!f) return -1;

  int itmp;
  char ctmp;
  size_t page_size = getpagesize();

  /* PID */
  if (fscanf(f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* Exec file */
  if (fscanf (f, "%s ", buf) == 0) goto error; /* coverity[secure_coding] */
  /* State */
  if (fscanf (f, "%c ", &ctmp) == 0) goto error; /* coverity[secure_coding] */
  /* Parent process */
  if (fscanf (f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* Process group */
  if (fscanf (f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* Session id */
  if (fscanf (f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* TTY */
  if (fscanf (f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* TTY owner process group */
  if (fscanf (f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* Flags */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* Minor faults (no memory page) */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* Minor faults, children */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* Major faults (memory page faults) */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* Major faults, children */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* utime */
  if (fscanf (f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* stime */
  if (fscanf (f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* utime, children */
  if (fscanf (f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* stime, children */
  if (fscanf (f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* jiffies remaining in current time slice */
  if (fscanf (f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* 'nice' value */
  if (fscanf (f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* jiffies until next timeout */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* jiffies until next SIGALRM */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* start time (jiffies since system boot) */
  if (fscanf (f, "%d ", &itmp) == 0) goto error; /* coverity[secure_coding] */

  /* Virtual memory size */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  *vsize = (size_t) itmp;

  /* Resident set size */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  *rss = (size_t) itmp * page_size;

  /* rlim */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* Start of text */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* End of text */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */
  /* Start of stack */
  if (fscanf (f, "%u ", &itmp) == 0) goto error; /* coverity[secure_coding] */

  fclose (f);

  return 0;

error:
  fclose (f);
  return -1;
}


int Platform::GetExecutablePath(char* buffer, size_t* size) {
  *size = readlink("/proc/self/exe", buffer, *size - 1);
  if (*size <= 0) return -1;
  buffer[*size] = '\0';
  return 0;
}

int Platform::GetCPUInfo(Local<Array> *cpus) {
  Local<Object> cpuinfo;
  Local<Object> cputimes;
  unsigned int ticks = (unsigned int)sysconf(_SC_CLK_TCK),
               multiplier = ((uint64_t)1000L / ticks), cpuspeed;
  int numcpus = 0, i = 0;
  unsigned long long ticks_user, ticks_sys, ticks_idle, ticks_nice, ticks_intr;
  char line[512], speedPath[256], model[512];
  FILE *fpStat = fopen("/proc/stat", "r");
  FILE *fpModel = fopen("/proc/cpuinfo", "r");
  FILE *fpSpeed;

  if (fpModel) {
    while (fgets(line, 511, fpModel) != NULL) {
      if (strncmp(line, "model name", 10) == 0) {
        numcpus++;
        if (numcpus == 1) {
          char *p = strchr(line, ':') + 2;
          strcpy(model, p);
          model[strlen(model)-1] = 0;
        }
      } else if (strncmp(line, "cpu MHz", 7) == 0) {
        if (numcpus == 1) {
          sscanf(line, "%*s %*s : %u", &cpuspeed);
        }
      }
    }
    fclose(fpModel);
  }

  *cpus = Array::New(numcpus);

  if (fpStat) {
    while (fgets(line, 511, fpStat) != NULL) {
      if (strncmp(line, "cpu ", 4) == 0)
        continue;
      else if (strncmp(line, "intr ", 5) == 0)
        break;
      sscanf(line, "%*s %llu %llu %llu %llu",
             &ticks_user, &ticks_nice, &ticks_sys, &ticks_idle);
      snprintf(speedPath, sizeof(speedPath),
               "/sys/devices/system/cpu/cpu%u/cpufreq/cpuinfo_max_freq", i);
      fpSpeed = fopen(speedPath, "r");
      if (fpSpeed) {
        if (fgets(line, 511, fpSpeed) != NULL) {
          sscanf(line, "%u", &cpuspeed);
          cpuspeed /= 1000;
        }
        fclose(fpSpeed);
      }
      cpuinfo = Object::New();
      cputimes = Object::New();
      cputimes->Set(String::New("user"), Number::New(ticks_user * multiplier));
      cputimes->Set(String::New("nice"), Number::New(ticks_nice * multiplier));
      cputimes->Set(String::New("sys"), Number::New(ticks_sys * multiplier));
      cputimes->Set(String::New("idle"), Number::New(ticks_idle * multiplier));
      cputimes->Set(String::New("irq"), Number::New(0));

      cpuinfo->Set(String::New("model"), String::New(model));
      cpuinfo->Set(String::New("speed"), Number::New(cpuspeed));

      cpuinfo->Set(String::New("times"), cputimes);
      (*cpus)->Set(i++, cpuinfo);
    }
    fclose(fpStat);
  }

  return 0;
}

double Platform::GetFreeMemory() {
  double pagesize = static_cast<double>(sysconf(_SC_PAGESIZE));
  double pages = static_cast<double>(sysconf(_SC_AVPHYS_PAGES));

  return static_cast<double>(pages * pagesize);
}

double Platform::GetTotalMemory() {
  double pagesize = static_cast<double>(sysconf(_SC_PAGESIZE));
  double pages = static_cast<double>(sysconf(_SC_PHYS_PAGES));

  return pages * pagesize;
}

double Platform::GetUptime() {
  double amount;
  char line[512];
  FILE *fpUptime = fopen("/proc/uptime", "r");

  if (fpUptime) {
    if (fgets(line, 511, fpUptime) != NULL) {
      sscanf(line, "%lf %*lf", &amount);
    }
    fclose(fpUptime);
  }

  return amount;
}

int Platform::GetLoadAvg(Local<Array> *loads) {
  // Unsupported as of cygwin 1.7.7
  return -1;
}

}  // namespace node
