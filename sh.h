#include "get_path.h"

int pid;
int sh( int argc, char **argv, char **envp);

/*** built-in functions ***/
char *which(char *command, struct pathelement *pathlist);
char *where(char *command, struct pathelement *pathlist);
void list ( char *dir );
void printenv(char **envp);



/*** helper functions ***/

/* takes the commandline and puts the command into command and args in args */
/* returns a non-negative integer for the number of args in args */
/* if no command, returns -1 */
int parse_command(char* commandline, char** args);

#define PROMPTMAX 32
#define MAXARGS 10
