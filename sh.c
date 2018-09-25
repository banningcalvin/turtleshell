#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"

int sh( int argc, char **argv, char **envp )
{
  //prefix to prompt, changed with 'prompt' command
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  //user input
  char *commandline = calloc(MAX_CANON, sizeof(char));
  //command = command that was run
  //arg = ???
  //commandpath = path to command (which(command, pathList))
  //p = ???
  //pwd = pointer to working dir from getcwd()
  //owd = copy of pwd???
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  //array of arguments of length argsct
  char **args = calloc(MAXARGS, sizeof(char*));
  //uid = username
  //i = ???iterator???
  //status = exit status int
  //argsct = number of args in args
  //go = whether prompt is still running(1) or not (0)
  int uid, i, status, argsct, go = 1;
  
  struct passwd *password_entry;
  
  char *homedir;
  
  struct pathelement *pathlist;

  uid = getuid();
  password_entry = getpwuid(uid); // get passwd info
  homedir = password_entry->pw_dir; // Home directory to start out with
     
  if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
    {
      perror("getcwd");
      exit(2);
    }
  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();


  /*******MAIN LOOP*******/
  while(go)
    {
      /* print prompt */
      printf("%s:%s» ", prompt, owd);
      fflush(NULL);
      
      /* get command line and process */
      if (!fgets(commandline, MAX_CANON, stdin)) {
	//if EOF or C-d, don't quit
	printf("\ntype \'exit\' to exit\n");
	continue;
      }
      /* process the command */
      /* command holds the command to be run, args holds arguments */
      /* if argsct is -1, command is null*/
      if((argsct = parse_command(commandline, &command, args)) == -1)
	continue;
      
      /* check for each built in command and implement */
      if(argsct != -1) {
	printf("run built in command\n");
      }
      /*  else  program to exec */
      else {
	/* find it /*
	   /* do fork(), execve() and waitpid() */
	if (1 == 2)
	  printf("do for, execve and waitpid\n");
	else
	  fprintf(stderr, "%s: Command not found.\n", args[0]);
      }
    }
  return 0;
} /* sh() */







/****************************************************************/
/********************** Built-in commands ***********************/
/****************************************************************/

char *which(char *command, struct pathelement *pathlist )
{
  /* loop through pathlist until finding command and return it.  Return
     NULL when not found. */

} /* which() */

char *where(char *command, struct pathelement *pathlist )
{
  /* similarly loop through finding all locations of command */
} /* where() */

void list ( char *dir )
{
  /* see man page for opendir() and readdir() and print out filenames for
     the directory passed */
} /* list() */



/****************************************************************/
/*********************** Helper functions ***********************/
/****************************************************************/

int parse_command(char* commandline, char** command, char** args) {
  commandline[strlen(commandline) - 1] = '\0'; // strip newline
  //saveptr for strtok_r() and delim char
  char* saveptr;
  const char* delim = " ";
  //temp string to hold the intermediate result from strtok_r
  char* temp;
  
  //extract command
  temp = strtok_r(commandline, delim, &saveptr);

  if(temp == NULL) {
    return -1;
  } else {
    //allocate memory for the command and put the string in it
    *command = malloc(sizeof(char) * (strlen(temp) + 1));
    strcpy(*command, temp);
    
    //extract args
    int i = 0;
    temp = strtok_r(NULL, delim, &saveptr);
    while (temp != NULL) {
      args[i] = malloc(sizeof(char) * (strlen(temp) + 1));
      strcpy(args[i], temp);
      i++;
      temp = strtok_r(NULL, delim, &saveptr);
    }
    return i;
  }
}
