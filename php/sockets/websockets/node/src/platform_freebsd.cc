#include "node.h"
#include "platform.h"

#include <v8.h>

#include <stdlib.h>
#include <kvm.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <sys/dkstat.h>
#include <vm/vm_param.h>
#include <string.h>
#include <paths.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>


namespace node {

using namespace v8;

static char *process_title;

char** Platform::SetupArgs(int argc, char *argv[]) {
  process_title = argc ? strdup(argv[0]) : NULL;
  return argv;
}


void Platform::SetProcessTitle(char *title) {
  if (process_title) free(process_title);
  process_title = strdup(title);
  setproctitle(title);
}

const char* Platform::GetProcessTitle(int *len) {
  if (process_title) {
    *len = strlen(process_title);
    return process_title;
  }
  *len = 0;
  return NULL;
}

int Platform::GetMemory(size_t *rss, size_t *vsize) {
  kvm_t *kd = NULL;
  struct kinfo_proc *kinfo = NULL;
  pid_t pid;
  int nprocs;
  size_t page_size = getpagesize();

  pid = getpid();

  kd = kvm_open(NULL, _PATH_DEVNULL, NULL, O_RDONLY, "kvm_open");
  if (kd == NULL) goto error;

  kinfo = kvm_getprocs(kd, KERN_PROC_PID, pid, &nprocs);
  if (kinfo == NULL) goto error;

  *rss = kinfo->ki_rssize * page_size;
  *vsize = kinfo->ki_size;

  kvm_close(kd);

  return 0;

error:
  if (kd) kvm_close(kd);
  return -1;
}


int Platform::GetExecutablePath(char* buffer, size_t* size) {
  int mib[4];
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;

  if (sysctl(mib, 4, buffer, size, NULL, 0) == -1) {
    return -1;
  }
  *size-=1;
  return 0;
}

int Platform::GetCPUInfo(Local<Array> *cpus) {
  Local<Object> cpuinfo;
  Local<Object> cputimes;
  unsigned int ticks = (unsigned int)sysconf(_SC_CLK_TCK),
               multiplier = ((uint64_t)1000L / ticks), cpuspeed, maxcpus,
               cur = 0;
  char model[512];
  int numcpus;
  size_t size;

  size = sizeof(model);
  if (sysctlbyname("hw.model", &model, &size, NULL, 0) < 0) {
    return -1;
  }
  size = sizeof(numcpus);
  if (sysctlbyname("hw.ncpu", &numcpus, &size, NULL, 0) < 0) {
    return -1;
  }

  *cpus = Array::New(numcpus);

  size = sizeof(cpuspeed);
  if (sysctlbyname("hw.clockrate", &cpuspeed, &size, NULL, 0) < 0) {
    return -1;
  }
  // kern.cp_times on FreeBSD i386 gives an array up to maxcpus instead of ncpu
  size = sizeof(maxcpus);
  if (sysctlbyname("kern.smp.maxcpus", &maxcpus, &size, NULL, 0) < 0) {
    return -1;
  }
  size = maxcpus * CPUSTATES * sizeof(long);
  long cp_times[size];
  if (sysctlbyname("kern.cp_times", &cp_times, &size, NULL, 0) < 0) {
    return -1;
  }
  for (int i = 0; i < numcpus; i++) {
    cpuinfo = Object::New();
    cputimes = Object::New();
    cputimes->Set(String::New("user"),
                  Number::New((uint64_t)(cp_times[CP_USER+cur]) * multiplier));
    cputimes->Set(String::New("nice"),
                  Number::New((uint64_t)(cp_times[CP_NICE+cur]) * multiplier));
    cputimes->Set(String::New("sys"),
                  Number::New((uint64_t)(cp_times[CP_SYS+cur]) * multiplier));
    cputimes->Set(String::New("idle"),
                  Number::New((uint64_t)(cp_times[CP_IDLE+cur]) * multiplier));
    cputimes->Set(String::New("irq"),
                  Number::New((uint64_t)(cp_times[CP_INTR+cur]) * multiplier));

    cpuinfo->Set(String::New("model"), String::New(model));
    cpuinfo->Set(String::New("speed"), Number::New(cpuspeed));

    cpuinfo->Set(String::New("times"), cputimes);
    (*cpus)->Set(i, cpuinfo);
    cur+=CPUSTATES;
  }

  return 0;
}

double Platform::GetFreeMemory() {
  double pagesize = static_cast<double>(sysconf(_SC_PAGESIZE));
  unsigned long info;
  size_t size = sizeof(info);

  if (sysctlbyname("vm.stats.vm.v_free_count", &info, &size, NULL, 0) < 0) {
    return -1;
  }

  return (static_cast<double>(info)) * pagesize;
}

double Platform::GetTotalMemory() {
#if defined(HW_PHYSMEM64)
  uint64_t info;
  static int which[] = {CTL_HW, HW_PHYSMEM64};
#else
  unsigned int info;
  static int which[] = {CTL_HW, HW_PHYSMEM};
#endif
  size_t size = sizeof(info);

  if (sysctl(which, 2, &info, &size, NULL, 0) < 0) {
    return -1;
  }

  return static_cast<double>(info);
}

double Platform::GetUptime() {
  time_t now;
  struct timeval info;
  size_t size = sizeof(info);
  static int which[] = {CTL_KERN, KERN_BOOTTIME};

  if (sysctl(which, 2, &info, &size, NULL, 0) < 0) {
    return -1;
  }
  now = time(NULL);

  return static_cast<double>(now - info.tv_sec);
}

int Platform::GetLoadAvg(Local<Array> *loads) {
  struct loadavg info;
  size_t size = sizeof(info);
  static int which[] = {CTL_VM, VM_LOADAVG};

  if (sysctl(which, 2, &info, &size, NULL, 0) < 0) {
    return -1;
  }
  (*loads)->Set(0, Number::New(static_cast<double>(info.ldavg[0])
                               / static_cast<double>(info.fscale)));
  (*loads)->Set(1, Number::New(static_cast<double>(info.ldavg[1])
                               / static_cast<double>(info.fscale)));
  (*loads)->Set(2, Number::New(static_cast<double>(info.ldavg[2])
                               / static_cast<double>(info.fscale)));

  return 0;
}

}  // namespace node
