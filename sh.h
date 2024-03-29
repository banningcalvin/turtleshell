#include "get_path.h"
#include "alias.h"
#include "history.h"
#include <pthread.h>


/* struct for sending args to pthreads */
struct threadargs {
  char** args; /* cli args */
  char* command; /* path to command */
  char** envp;
};

/*** pthread execution functions ***/
void* pthread_exec_path(void *arg);
void* pthread_exec_external(void *arg);

int pid;
int sh( int argc, char **argv, char **envp);

/*** built-in functions ***/

/* loop through pathlist until finding command and return it.  Return
     NULL when not found. */
char *which(char *command, struct pathelement *pathlist);
/* similarly loop through finding all locations of command */
char *where(char *command, struct pathelement *pathlist);
/* change directory
 * ~ or no arg goes to homedir
 * - goes to pwd and sets owd to pwd and pwd to owd
 * a valid path as an arg sets owd to that path and pwd to owd
 */
void cd(char **owd, char **pwd, char *homedir, char *arg);
/* see man page for opendir() and readdir() and print out filenames for
     the directory passed */
void list (char *dir);

/*** helper functions ***/

/* takes the commandline and puts the command into command and args in args */
/* returns a non-negative integer for the number of args in args */
/* if no command, returns -1 */
int parse_command(char* commandline, char** args);
/* empty argsct number of  args */
void blank_args(int argsct, char **args);
/* returns 0 if a path is absolute, 1 if it is not (./ls vs ls) */
int is_absolute(char *command);

void *watchuser(void *arg);
 void *watchmail(void *arg);

struct maillist{
  char *str;
  pthread_t id;
  struct maillist *next;
};
  
struct strlist{
  char *str;
  int status;
  struct strlist *next;
  struct strlist *prev;
};

#define PROMPTMAX 32
#define MAXARGS 10
