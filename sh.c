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
  //command = ???
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
      if((argsct = parse_command(commandline, args)) == 0)
	continue;

      /* check for each built in command and implement */

      //if command is exit with 0 args, exit
      if(strcmp(args[0], "exit") == 0) {
	if (argsct == 1) {
	  go = 0;
	  break;
	} else {
	  printf("%s: incorrect number of args, 0 expected\n", command);
	  continue;
	}
      } else if(strcmp(args[0], "which") == 0) {

      } else if(strcmp(args[0], "where") == 0) {

      } else if(strcmp(args[0], "cd") == 0) {

      } else if(strcmp(args[0], "pwd") == 0) {

      } else if(strcmp(args[0], "list") == 0) {

      } else if(strcmp(args[0], "pid") == 0) {

      } else if(strcmp(args[0], "kill") == 0) {

      } else if(strcmp(args[0], "prompt") == 0) {

      } else if(strcmp(args[0], "printenv") == 0) {

      } else if(strcmp(args[0], "alias") == 0) {

      } else if(strcmp(args[0], "history") == 0) {

      } else if(strcmp(args[0], "setenv") == 0) {

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
  printf("exit\n");

  // TODO
  // RUN EXIT CODE HERE
  
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

int parse_command(char* commandline, char** args) {
  commandline[strlen(commandline) - 1] = '\0'; // strip newline
  //saveptr for strtok_r() and delim char
  char* saveptr;
  const char* delim = " ";
  //temp string to hold the intermediate result from strtok_r
  char* temp;
  
  //extract command
  temp = strtok_r(commandline, delim, &saveptr);

  if(temp == NULL) {
    return 0;
  } else {
    //allocate memory for the command and put the string in it
    args[0] = malloc(sizeof(char) * (strlen(temp) + 1));
    strcpy(args[0], temp);
    
    //extract args
    int i = 1;
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
