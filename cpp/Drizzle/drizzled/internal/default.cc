/* Copyright (C) 2000-2003 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

/****************************************************************************
 Add all options from files named "group".cnf from the default_directories
 before the command line arguments.
 On Windows defaults will also search in the Windows directory for a file
 called 'group'.ini
 As long as the program uses the last argument for conflicting
 options one only have to add a call to "load_defaults" to enable
 use of default values.
 pre- and end 'blank space' are removed from options and values. The
 following escape sequences are recognized in values:  \b \t \n \r \\

 The following arguments are handled automaticly;  If used, they must be
 first argument on the command line!
 --no-defaults	; no options are read.
 --defaults-file=full-path-to-default-file	; Only this file will be read.
 --defaults-extra-file=full-path-to-default-file ; Read this file before ~/
 --defaults-group-suffix  ; Also read groups with concat(group, suffix)
 --print-defaults	  ; Print the modified command line and exit
****************************************************************************/

#include <config.h>

#include <drizzled/internal/my_sys.h>
#include <drizzled/internal/m_string.h>
#include <drizzled/charset_info.h>
#include <drizzled/typelib.h>
#include <drizzled/configmake.h>
#include <drizzled/gettext.h>
#include <drizzled/dynamic_array.h>
#include <drizzled/cached_directory.h>

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#include <cstdio>
#include <algorithm>

using namespace std;

namespace drizzled
{
namespace internal
{

const char *my_defaults_file=0;
const char *my_defaults_group_suffix=0;
char *my_defaults_extra_file=0;

/* Which directories are searched for options (and in which order) */

#define MAX_DEFAULT_DIRS 6
const char *default_directories[MAX_DEFAULT_DIRS + 1];

static const char *f_extensions[]= { ".cnf", 0 };

int handle_default_option(void *in_ctx, const char *group_name,
                          const char *option);

/*
   This structure defines the context that we pass to callback
   function 'handle_default_option' used in search_default_file
   to process each option. This context is used if search_default_file
   was called from load_defaults.
*/

struct handle_option_ctx
{
   memory::Root *alloc;
   DYNAMIC_ARRAY *args;
   TYPELIB *group;
};

static int search_default_file(Process_option_func func, void *func_ctx,
			       const char *dir, const char *config_file);
static int search_default_file_with_ext(Process_option_func func,
                                        void *func_ctx,
					const char *dir, const char *ext,
					const char *config_file, int recursion_level);



/**
  Create the list of default directories.

  @details
  On all systems, if a directory is already in the list, it will be moved
  to the end of the list.  This avoids reading defaults files multiple times,
  while ensuring the correct precedence.

  @return void
*/

static void init_default_directories(void);


static char *remove_end_comment(char *ptr);


/*
  Process config files in default directories.

  SYNOPSIS
  my_search_option_files()
  conf_file                   Basename for configuration file to search for.
                              If this is a path, then only this file is read.
  argc                        Pointer to argc of original program
  argv                        Pointer to argv of original program
  args_used                   Pointer to variable for storing the number of
                              arguments used.
  func                        Pointer to the function to process options
  func_ctx                    It's context. Usually it is the structure to
                              store additional options.
  DESCRIPTION
    Process the default options from argc & argv
    Read through each found config file looks and calls 'func' to process
    each option.

  NOTES
    --defaults-group-suffix is only processed if we are called from
    load_defaults().


  RETURN
    0  ok
    1  given cinf_file doesn't exist

    The global variable 'my_defaults_group_suffix' is updated with value for
    --defaults_group_suffix
*/

int my_search_option_files(const char *conf_file, int *argc, char ***argv,
                           uint32_t *args_used, Process_option_func func,
                           void *func_ctx)
{
  const char **dirs, *forced_default_file, *forced_extra_defaults;
  int error= 0;

  /* Check if we want to force the use a specific default file */
  *args_used+= get_defaults_options(*argc - *args_used, *argv + *args_used,
                                    (char **) &forced_default_file,
                                    (char **) &forced_extra_defaults,
                                    (char **) &my_defaults_group_suffix);

  if (! my_defaults_group_suffix)
    my_defaults_group_suffix= getenv("DRIZZLE_GROUP_SUFFIX");

  if (forced_extra_defaults)
    my_defaults_extra_file= (char *) forced_extra_defaults;

  if (forced_default_file)
    my_defaults_file= forced_default_file;

  /*
    We can only handle 'defaults-group-suffix' if we are called from
    load_defaults() as otherwise we can't know the type of 'func_ctx'
  */

  if (my_defaults_group_suffix && (func == handle_default_option))
  {
    /* Handle --defaults-group-suffix= */
    uint32_t i;
    const char **extra_groups;
    const size_t instance_len= strlen(my_defaults_group_suffix);
    struct handle_option_ctx *ctx= (struct handle_option_ctx*) func_ctx;
    char *ptr;
    TYPELIB *group= ctx->group;

    if (!(extra_groups=
	  (const char**)ctx->alloc->alloc_root(
                                   (2*group->count+1)*sizeof(char*))))
      goto err;

    for (i= 0; i < group->count; i++)
    {
      size_t len;
      extra_groups[i]= group->type_names[i]; /** copy group */

      len= strlen(extra_groups[i]);
      if (!(ptr= (char *)ctx->alloc->alloc_root( len+instance_len+1)))
	goto err;

      extra_groups[i+group->count]= ptr;

      /** Construct new group */
      memcpy(ptr, extra_groups[i], len);
      memcpy(ptr+len, my_defaults_group_suffix, instance_len+1);
    }

    group->count*= 2;
    group->type_names= extra_groups;
    group->type_names[group->count]= 0;
  }

  if (forced_default_file)
  {
    if ((error= search_default_file_with_ext(func, func_ctx, "", "",
                                             forced_default_file, 0)) < 0)
      goto err;
    if (error > 0)
    {
      fprintf(stderr, "Could not open required defaults file: %s\n",
              forced_default_file);
      goto err;
    }
  }
  else if (dirname_length(conf_file))
  {
    if ((error= search_default_file(func, func_ctx, NULL, conf_file)) < 0)
      goto err;
  }
  else
  {
    for (dirs= default_directories ; *dirs; dirs++)
    {
      if (**dirs)
      {
	if (search_default_file(func, func_ctx, *dirs, conf_file) < 0)
	  goto err;
      }
      else if (my_defaults_extra_file)
      {
        if ((error= search_default_file_with_ext(func, func_ctx, "", "",
                                                my_defaults_extra_file, 0)) < 0)
	  goto err;				/* Fatal error */
        if (error > 0)
        {
          fprintf(stderr, "Could not open required defaults file: %s\n",
                  my_defaults_extra_file);
          goto err;
        }
      }
    }
  }

  return(error);

err:
  fprintf(stderr,"Fatal error in defaults handling. Program aborted\n");
  exit(1);
}


/*
  The option handler for load_defaults.

  SYNOPSIS
    handle_deault_option()
    in_ctx                  Handler context. In this case it is a
                            handle_option_ctx structure.
    group_name              The name of the group the option belongs to.
    option                  The very option to be processed. It is already
                            prepared to be used in argv (has -- prefix). If it
                            is NULL, we are handling a new group (section).

  DESCRIPTION
    This handler checks whether a group is one of the listed and adds an option
    to the array if yes. Some other handler can record, for instance, all
    groups and their options, not knowing in advance the names and amount of
    groups.

  RETURN
    0 - ok
    1 - error occured
*/

int handle_default_option(void *in_ctx, const char *group_name,
                          const char *option)
{
  char *tmp;
  struct handle_option_ctx *ctx= (struct handle_option_ctx *) in_ctx;

  if (!option)
    return 0;

  if (ctx->group->find_type(const_cast<char*>(group_name), 3))
  {
    if (!(tmp= (char *)ctx->alloc->alloc_root(strlen(option) + 1)))
      return 1;
    ctx->args->push_back(&tmp);
    strcpy(tmp, option);
  }

  return 0;
}


/*
  Gets options from the command line

  SYNOPSIS
    get_defaults_options()
    argc			Pointer to argc of original program
    argv			Pointer to argv of original program
    defaults                    --defaults-file option
    extra_defaults              --defaults-extra-file option

  RETURN
    # Number of arguments used from *argv
      defaults and extra_defaults will be set to option of the appropriate
      items of argv array, or to NULL if there are no such options
*/

int get_defaults_options(int argc, char **argv,
                         char **defaults,
                         char **extra_defaults,
                         char **group_suffix)
{
  int org_argc= argc, prev_argc= 0;
  *defaults= *extra_defaults= *group_suffix= 0;

  const std::string DEFAULTS_FILE("--defaults-file=");
  const std::string DEFAULTS_EXTRA_FILE("--defaults-extra-file=");
  const std::string DEFAULTS_GROUP_SUFFIX("--defaults-group-suffix=");

  while (argc >= 2 && argc != prev_argc)
  {
    /* Skip program name or previously handled argument */
    argv++;
    prev_argc= argc;                            /* To check if we found */
    if (!*defaults && (strncmp(*argv,
                               DEFAULTS_FILE.c_str(),
                               DEFAULTS_FILE.size()) == 0))
    {
      *defaults= *argv + DEFAULTS_FILE.size();
       argc--;
       continue;
    }
    if (!*extra_defaults && (strncmp(*argv, 
                                     DEFAULTS_EXTRA_FILE.c_str(),
                                     DEFAULTS_EXTRA_FILE.size()) == 0))
    {
      *extra_defaults= *argv + DEFAULTS_EXTRA_FILE.size();
      argc--;
      continue;
    }
    if (!*group_suffix && (strncmp(*argv, 
                                   DEFAULTS_GROUP_SUFFIX.c_str(),
                                   DEFAULTS_GROUP_SUFFIX.size()) == 0))

    {
      *group_suffix= *argv + DEFAULTS_GROUP_SUFFIX.size();
      argc--;
      continue;
    }
  }
  return org_argc - argc;
}


/*
  Read options from configurations files

  SYNOPSIS
    load_defaults()
    conf_file			Basename for configuration file to search for.
    				If this is a path, then only this file is read.
    groups			Which [group] entrys to read.
				Points to an null terminated array of pointers
    argc			Pointer to argc of original program
    argv			Pointer to argv of original program

  IMPLEMENTATION

   Read options from configuration files and put them BEFORE the arguments
   that are already in argc and argv.  This way the calling program can
   easily command line options override options in configuration files

   NOTES
    In case of fatal error, the function will print a warning and do
    exit(1)

    To free used memory one should call free_defaults() with the argument
    that was put in *argv

   RETURN
     0	ok
     1	The given conf_file didn't exists
*/


int load_defaults(const char *conf_file, const char **groups,
                  int *argc, char ***argv)
{
  DYNAMIC_ARRAY args;
  TYPELIB group;
  uint32_t args_used= 0;
  int error= 0;
  memory::Root alloc(512);
  char *ptr,**res;
  struct handle_option_ctx ctx;

  init_default_directories();
  /*
    Check if the user doesn't want any default option processing
    --no-defaults is always the first option
  */
  if (*argc >= 2 && !strcmp(argv[0][1],"--no-defaults"))
  {
    /* remove the --no-defaults argument and return only the other arguments */
    uint32_t i;
    if (!(ptr=(char*) alloc.alloc_root(sizeof(alloc)+ (*argc + 1)*sizeof(char*))))
      goto err;
    res= (char**) (ptr+sizeof(alloc));
    memset(res,0,(*argc + 1));
    res[0]= **argv;				/* Copy program name */
    for (i=2 ; i < (uint32_t) *argc ; i++)
      res[i-1]=argv[0][i];
    res[i-1]=0;					/* End pointer */
    (*argc)--;
    *argv=res;
    *(memory::Root*) ptr= alloc;			/* Save alloc root for free */
    return(0);
  }

  group.count=0;
  group.name= "defaults";
  group.type_names= groups;

  for (; *groups ; groups++)
    group.count++;

  if (my_init_dynamic_array(&args, sizeof(char*),*argc, 32))
    goto err;

  ctx.alloc= &alloc;
  ctx.args= &args;
  ctx.group= &group;

  error= my_search_option_files(conf_file, argc, argv, &args_used,
                                handle_default_option, (void *) &ctx);
  /*
    Here error contains <> 0 only if we have a fully specified conf_file
    or a forced default file
  */
  if (!(ptr=(char*) alloc.alloc_root(sizeof(alloc)+ (args.size() + *argc +1) *sizeof(char*))))
    goto err;
  res= (char**) (ptr+sizeof(alloc));

  /* copy name + found arguments + command line arguments to new array */
  res[0]= argv[0][0];  /* Name MUST be set, even by embedded library */
  memcpy(res+1, args.buffer, args.size()*sizeof(char*));
  /* Skip --defaults-xxx options */
  (*argc)-= args_used;
  (*argv)+= args_used;

  /*
    Check if we wan't to see the new argument list
    This options must always be the last of the default options
  */
  if (*argc)
    memcpy(res+1+args.size(), *argv + 1, (*argc-1)*sizeof(char*));
  res[args.size()+ *argc]=0;			/* last null */

  (*argc)+=int(args.size());
  *argv= static_cast<char**>(res);
  *(memory::Root*) ptr= alloc;			/* Save alloc root for free */
  delete_dynamic(&args);

  return(error);

 err:
  fprintf(stderr,"Fatal error in defaults handling. Program aborted\n");
  exit(1);
}


void free_defaults(char **argv)
{
  memory::Root ptr;
  memcpy(&ptr, (char*) argv - sizeof(ptr), sizeof(ptr));
  ptr.free_root(MYF(0));
}


static int search_default_file(Process_option_func opt_handler,
                               void *handler_ctx,
			       const char *dir,
			       const char *config_file)
{
  char **ext;
  const char *empty_list[]= { "", 0 };
  bool have_ext= fn_ext(config_file)[0] != 0;
  const char **exts_to_use= have_ext ? empty_list : f_extensions;

  for (ext= (char**) exts_to_use; *ext; ext++)
  {
    int error;
    if ((error= search_default_file_with_ext(opt_handler, handler_ctx,
                                             dir, *ext,
					     config_file, 0)) < 0)
      return error;
  }
  return 0;
}


/*
  Skip over keyword and get argument after keyword

  SYNOPSIS
   get_argument()
   keyword		Include directive keyword
   kwlen		Length of keyword
   ptr			Pointer to the keword in the line under process
   line			line number

  RETURN
   0	error
   #	Returns pointer to the argument after the keyword.
*/

static char *get_argument(const char *keyword, size_t kwlen,
                          char *ptr, char *name, uint32_t line)
{
  char *end;

  /* Skip over "include / includedir keyword" and following whitespace */

  for (ptr+= kwlen - 1;
       my_isspace(&my_charset_utf8_general_ci, ptr[0]);
       ptr++)
  {}

  /*
    Trim trailing whitespace from directory name
    The -1 below is for the newline added by fgets()
    Note that my_isspace() is true for \r and \n
  */
  for (end= ptr + strlen(ptr) - 1;
       my_isspace(&my_charset_utf8_general_ci, *(end - 1));
       end--)
  {}
  end[0]= 0;                                    /* Cut off end space */

  /* Print error msg if there is nothing after !include* directive */
  if (end <= ptr)
  {
    fprintf(stderr,
	    "error: Wrong '!%s' directive in config file: %s at line %d\n",
	    keyword, name, line);
    return 0;
  }
  return ptr;
}


/*
  Open a configuration file (if exists) and read given options from it

  SYNOPSIS
    search_default_file_with_ext()
    opt_handler                 Option handler function. It is used to process
                                every separate option.
    handler_ctx                 Pointer to the structure to store actual
                                parameters of the function.
    dir				directory to read
    ext				Extension for configuration file
    config_file                 Name of configuration file
    group			groups to read
    recursion_level             the level of recursion, got while processing
                                "!include" or "!includedir"

  RETURN
    0   Success
    -1	Fatal error, abort
     1	File not found (Warning)
*/

static int search_default_file_with_ext(Process_option_func opt_handler,
                                        void *handler_ctx,
                                        const char *dir,
                                        const char *ext,
                                        const char *config_file,
                                        int recursion_level)
{
  char name[FN_REFLEN + 10], buff[4096], curr_gr[4096], *ptr, *end, **tmp_ext;
  char *value, option[4096], tmp[FN_REFLEN];
  static const char includedir_keyword[]= "includedir";
  static const char include_keyword[]= "include";
  const int max_recursion_level= 10;
  FILE *fp;
  uint32_t line=0;
  bool found_group=0;

  if ((dir ? strlen(dir) : 0 )+strlen(config_file) >= FN_REFLEN-3)
    return 0;					/* Ignore wrong paths */
  if (dir)
  {
    end=convert_dirname(name, dir, NULL);
    if (dir[0] == FN_HOMELIB)		/* Add . to filenames in home */
      *end++='.';
    sprintf(end,"%s%s",config_file,ext);
  }
  else
  {
    strcpy(name,config_file);
  }
  fn_format(name,name,"","",4);
  {
    struct stat stat_info;
    if (stat(name,&stat_info))
      return 1;
    /*
      Ignore world-writable regular files.
      This is mainly done to protect us to not read a file created by
      the mysqld server, but the check is still valid in most context.
    */
    if ((stat_info.st_mode & S_IWOTH) &&
	(stat_info.st_mode & S_IFMT) == S_IFREG)
    {
      fprintf(stderr, "Warning: World-writable config file '%s' is ignored\n",
              name);
      return 0;
    }
  }
  if (!(fp= fopen(name, "r")))
    return 1;					/* Ignore wrong files */

  memset(buff,0,sizeof(buff));
  while (fgets(buff, sizeof(buff) - 1, fp))
  {
    line++;
    /* Ignore comment and empty lines */
    for (ptr= buff; my_isspace(&my_charset_utf8_general_ci, *ptr); ptr++)
    {}

    if (*ptr == '#' || *ptr == ';' || !*ptr)
      continue;

    /* Configuration File Directives */
    if ((*ptr == '!'))
    {
      if (recursion_level >= max_recursion_level)
      {
        for (end= ptr + strlen(ptr) - 1;
             my_isspace(&my_charset_utf8_general_ci, *(end - 1));
             end--)
        {}
        end[0]= 0;
        fprintf(stderr,
                "Warning: skipping '%s' directive as maximum include"
                "recursion level was reached in file %s at line %d\n",
                ptr, name, line);
        continue;
      }

      /* skip over `!' and following whitespace */
      for (++ptr; my_isspace(&my_charset_utf8_general_ci, ptr[0]); ptr++)
      {}

      if ((!strncmp(ptr, includedir_keyword,
                    sizeof(includedir_keyword) - 1)) &&
          my_isspace(&my_charset_utf8_general_ci, ptr[sizeof(includedir_keyword) - 1]))
      {
	if (!(ptr= get_argument(includedir_keyword,
                                sizeof(includedir_keyword),
                                ptr, name, line)))
	  goto err;

        CachedDirectory dir_cache(ptr);

        if (dir_cache.fail())
        {
          /**
           * @todo
           * Since clients still use this code, we use fprintf here.
           * This fprintf needs to be turned into errmsg_printf
           * as soon as the client programs no longer use mysys
           * and can use the pluggable error message system.
           */
          fprintf(stderr, _("error: could not open directory: %s\n"), ptr);
          goto err;
        }

        CachedDirectory::Entries files= dir_cache.getEntries();
        CachedDirectory::Entries::iterator file_iter= files.begin();

        while (file_iter != files.end())
        {
          CachedDirectory::Entry *entry= *file_iter;
          ext= fn_ext(entry->filename.c_str());

          /* check extension */
          for (tmp_ext= (char**) f_extensions; *tmp_ext; tmp_ext++)
          {
            if (!strcmp(ext, *tmp_ext))
            {
              fn_format(tmp, entry->filename.c_str(), ptr, "",
                        MY_UNPACK_FILENAME | MY_SAFE_PATH);

              search_default_file_with_ext(opt_handler, handler_ctx, "", "",
                                           tmp, recursion_level + 1);
            }
          }

          ++file_iter;
        }
      }
      else if ((!strncmp(ptr, include_keyword, sizeof(include_keyword) - 1)) &&
               my_isspace(&my_charset_utf8_general_ci, ptr[sizeof(include_keyword)-1]))
      {
	if (!(ptr= get_argument(include_keyword,
                                sizeof(include_keyword), ptr,
                                name, line)))
	  goto err;

        search_default_file_with_ext(opt_handler, handler_ctx, "", "", ptr,
                                     recursion_level + 1);
      }

      continue;
    }

    if (*ptr == '[')				/* Group name */
    {
      found_group=1;
      if (!(end=(char *) strchr(++ptr,']')))
      {
	fprintf(stderr,
		"error: Wrong group definition in config file: %s at line %d\n",
		name,line);
	goto err;
      }
      /* Remove end space */
      for ( ; my_isspace(&my_charset_utf8_general_ci,end[-1]) ; end--) ;
      end[0]=0;

      strncpy(curr_gr, ptr, min((size_t) (end-ptr)+1, sizeof(curr_gr)-1));
      curr_gr[min((size_t)(end-ptr)+1, sizeof(curr_gr)-1)] = '\0';

      /* signal that a new group is found */
      opt_handler(handler_ctx, curr_gr, NULL);

      continue;
    }
    if (!found_group)
    {
      fprintf(stderr,
	      "error: Found option without preceding group in config file: %s at line: %d\n",
	      name,line);
      goto err;
    }


    end= remove_end_comment(ptr);
    if ((value= strchr(ptr, '=')))
      end= value;				/* Option without argument */
    for ( ; my_isspace(&my_charset_utf8_general_ci,end[-1]) || end[-1]== '\n'; end--) ;
    if (!value)
    {
      strncpy(strcpy(option,"--")+2,ptr,strlen(ptr)+1);
      if (opt_handler(handler_ctx, curr_gr, option))
        goto err;
    }
    else
    {
      /* Remove pre- and end space */
      char *value_end;
      for (value++ ; my_isspace(&my_charset_utf8_general_ci,*value); value++) ;
      value_end= strchr(value, '\0');
      /*
       We don't have to test for value_end >= value as we know there is
       an '=' before
      */
      for ( ; my_isspace(&my_charset_utf8_general_ci,value_end[-1]) ; value_end--) ;
      if (value_end < value)			/* Empty string */
        value_end=value;

      /* remove quotes around argument */
      if ((*value == '\"' || *value == '\'') && /* First char is quote */
          (value + 1 < value_end ) && /* String is longer than 1 */
          *value == value_end[-1] ) /* First char is equal to last char */
      {
        value++;
        value_end--;
      }
      
      memset(option,0,2+(size_t)(end-ptr)+1);
      ptr= strncpy(strcpy(option,"--")+2,ptr,(size_t) (end-ptr));
      ptr[end-ptr]= '\0';
      ptr+= strlen(ptr);
      *ptr++= '=';

      for ( ; value != value_end; value++)
      {
	if (*value == '\\' && value != value_end-1)
	{
	  switch(*++value) {
	  case 'n':
	    *ptr++='\n';
	    break;
	  case 't':
	    *ptr++= '\t';
	    break;
	  case 'r':
	    *ptr++ = '\r';
	    break;
	  case 'b':
	    *ptr++ = '\b';
	    break;
	  case 's':
	    *ptr++= ' ';			/* space */
	    break;
	  case '\"':
	    *ptr++= '\"';
	    break;
	  case '\'':
	    *ptr++= '\'';
	    break;
	  case '\\':
	    *ptr++= '\\';
	    break;
	  default:				/* Unknown; Keep '\' */
	    *ptr++= '\\';
	    *ptr++= *value;
	    break;
	  }
	}
	else
	  *ptr++= *value;
      }
      *ptr=0;
      if (opt_handler(handler_ctx, curr_gr, option))
        goto err;
    }
  }
  fclose(fp);
  return(0);

 err:
  fclose(fp);

  return -1;					/* Fatal error */
}


static char *remove_end_comment(char *ptr)
{
  char quote= 0;	/* we are inside quote marks */
  char escape= 0;	/* symbol is protected by escape chagacter */

  for (; *ptr; ptr++)
  {
    if ((*ptr == '\'' || *ptr == '\"') && !escape)
    {
      if (!quote)
	quote= *ptr;
      else if (quote == *ptr)
	quote= 0;
    }
    /* We are not inside a string */
    if (!quote && *ptr == '#')
    {
      *ptr= 0;
      return ptr;
    }
    escape= (quote && *ptr == '\\' && !escape);
  }
  return ptr;
}

void my_print_default_files(const char *conf_file)
{
  const char *empty_list[]= { "", 0 };
  bool have_ext= fn_ext(conf_file)[0] != 0;
  const char **exts_to_use= have_ext ? empty_list : f_extensions;
  char name[FN_REFLEN], **ext;
  const char **dirs;

  init_default_directories();
  puts("\nDefault options are read from the following files in the given order:");

  if (dirname_length(conf_file))
    fputs(conf_file,stdout);
  else
  {
    for (dirs=default_directories ; *dirs; dirs++)
    {
      for (ext= (char**) exts_to_use; *ext; ext++)
      {
	const char *pos;
	char *end;
	if (**dirs)
	  pos= *dirs;
	else if (my_defaults_extra_file)
	  pos= my_defaults_extra_file;
	else
	  continue;
	end= convert_dirname(name, pos, NULL);
	if (name[0] == FN_HOMELIB)	/* Add . to filenames in home */
	  *end++='.';
  sprintf(end,"%s%s ",conf_file, *ext);
	fputs(name,stdout);
      }
    }
  }
  puts("");
}

void print_defaults(const char *conf_file, const char **groups)
{
  const char **groups_save= groups;
  my_print_default_files(conf_file);

  fputs("The following groups are read:",stdout);
  for ( ; *groups ; groups++)
  {
    fputc(' ',stdout);
    fputs(*groups,stdout);
  }

  if (my_defaults_group_suffix)
  {
    groups= groups_save;
    for ( ; *groups ; groups++)
    {
      fputc(' ',stdout);
      fputs(*groups,stdout);
      fputs(my_defaults_group_suffix,stdout);
    }
  }
  puts("\nThe following options may be given as the first argument:\n\
  --no-defaults		Don't read default options from any options file\n\
  --defaults-file=#	Only read default options from the given file #\n\
  --defaults-extra-file=# Read this file after the global files are read");
}

/*
  This extra complexity is to avoid declaring 'rc' if it won't be
  used.
*/
static void add_directory(const char* dir)
{
  array_append_string_unique(dir, default_directories, array_elements(default_directories));
}

static void add_common_directories()
{
  const char *env= getenv("DRIZZLE_HOME"); 
  if (env) 
    add_directory(env); 
  // Placeholder for --defaults-extra-file=<path>
  add_directory(""); 
}

/**
  Initialize default directories for Unix

  @details
    1. /etc/
    2. /etc/drizzle/
    3. --sysconfdir=<path> (compile-time option)
    4. getenv("DRIZZLE_HOME")
    5. --defaults-extra-file=<path> (run-time option)
    6. "~/"
*/

static void init_default_directories(void)
{
  memset(default_directories, 0, sizeof(default_directories));
  add_directory("/etc/");
  add_directory("/etc/drizzle/");
  add_directory(SYSCONFDIR);
  add_common_directories();
  add_directory("~/");
}

} /* namespace internal */
} /* namespace drizzled */
