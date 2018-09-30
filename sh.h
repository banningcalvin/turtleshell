#include "get_path.h"

int pid;
int sh( int argc, char **argv, char **envp);

/*** built-in functions ***/

/* loop through pathlist until finding command and return it.  Return
     NULL when not found. */
char *which(char *command, struct pathelement *pathlist);
/* similarly loop through finding all locations of command */
char *where(char *command, struct pathelement *pathlist);
/* see man page for opendir() and readdir() and print out filenames for
     the directory passed */
void list ( char *dir );
void printenv(char **envp);



/*** helper functions ***/

/* takes the commandline and puts the command into command and args in args */
/* returns a non-negative integer for the number of args in args */
/* if no command, returns -1 */
int parse_command(char* commandline, char** args);
/* empty argsct number of  args */
void blank_args(int argsct, char **args);
//catches C-c and C-z
void siginthandler(int sig_num);
void sigtstphandler(int sig_num);

#define PROMPTMAX 32
#define MAXARGS 10
