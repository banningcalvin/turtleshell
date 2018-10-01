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

/* see header file for function descriptions */

int sh( int argc, char **argv, char **envp )
{
  /* prefix to prompt, changed with 'prompt' command */
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  /* user input */
  char *commandline = calloc(MAX_CANON, sizeof(char));
  /* command = ??? */
  /*  //arg = ??? */
  /* commandpath = path to command (which(command, pathList)) */
  /* p = ??? */
  /* pwd = previous working directory */
  /* owd = copy of pwd??? */
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  /* array of arguments of length argsct */
  char **args = calloc(MAXARGS, sizeof(char*));
  /* uid = username */
  /* i = ???iterator??? */
  /* status = exit status int */
  /* argsct = number of args in args */
  /* go = whether prompt is still running(1) or not (0) */
  int uid, i, status, argsct, go = 1;
  
  struct passwd *password_entry;
  
  char *homedir;
  
  struct pathelement *pathlist;

  uid = getuid();
  password_entry = getpwuid(uid); /* get passwd info */
  homedir = password_entry->pw_dir; /* Home directory to start out with */
  
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
	/* if EOF or C-d, don't quit */
	printf("\ntype \'exit\' to exit\n");
	continue;
      }
      
      /* process the command */
      /* command holds the command to be run, args holds arguments */
      /* if argsct is -1, command is null*/
      if((argsct = parse_command(commandline, args)) == 0)
	continue;

      /* check for each built in command and implement */

      /* if command is exit with 0 args, exit */
      if(strcmp(args[0], "exit") == 0) {
	if (argsct == 1) {
	  go = 0;
	  break;
	} else {
	  printf("%s: incorrect number of args, 0 expected\n", args[0]);
	  blank_args(argsct, args);
	  continue;
	}
      } else if(strcmp(args[0], "which") == 0) { /********************* which */
	printf("Executing built-in command %s\n", args[0]);
	if(argsct < 2) {
	  printf("%s: incorrect number of args, at least 1 expected\n", args[0]);
	} else {
	  for(i = 1; i < argsct; i++) {
	    command = which(args[i], pathlist);
	    if(command != NULL) {
	      printf("%s\n", command);
	      free(command);
	    }
	  }
	}
      } else if(strcmp(args[0], "where") == 0) { /********************* where */
	printf("Executing built-in command %s\n", args[0]);
	if(argsct < 2) {
	  printf("%s: incorrect number of args, at least 1 expected\n", args[0]);
	} else {
	  for(i = 1; i < argsct; i++) {
	    command = where(args[i], pathlist);
	    if(command != NULL) {
	      printf("%s", command);
	      free(command);
	    }
	  }
	}
      } else if(strcmp(args[0], "cd") == 0) { /*************************** cd */
	printf("Executing built-in command %s\n", args[0]);
	if(argsct > 2) {
	  printf("%s: incorrect number of args, at most 1 expected\n", args[0]);
	} else {
	  cd(&owd, &pwd, homedir);
	}
      } else if(strcmp(args[0], "pwd") == 0) { /************************* pwd */
	printf("Executing built-in command %s\n", args[0]);
	if(argsct > 1)
	  printf("%s: ignoring non-option arguments\n", args[0]);
	printf("%s/\n", owd);
      } else if(strcmp(args[0], "list") == 0) { /*********************** list */
	printf("Executing built-in command %s\n", args[0]);
	if(argsct == 1) {
	  list(owd);
	} else {
	  for(i = 1; i < argsct; i++) {
	    printf("contents of %s:\n", args[i]);
	    list(args[i]);
	  }
	}
      } else if(strcmp(args[0], "pid") == 0) {
	printf("Executing built-in command %s\n", args[0]);
	
      } else if(strcmp(args[0], "kill") == 0) {
	printf("Executing built-in command %s\n", args[0]);
	
      } else if(strcmp(args[0], "prompt") == 0) {
	printf("Executing built-in command %s\n", args[0]);
	
      } else if(strcmp(args[0], "printenv") == 0) {
	printf("Executing built-in command %s\n", args[0]);
	
      } else if(strcmp(args[0], "alias") == 0) {
	printf("Executing built-in command %s\n", args[0]);
	
      } else if(strcmp(args[0], "history") == 0) {
	printf("Executing built-in command %s\n", args[0]);
	
      } else if(strcmp(args[0], "setenv") == 0) {
	printf("Executing built-in command %s\n", args[0]);
	
      }
      /*  else  program to exec */
      else {
	/* find it */
	   /* do fork(), execve() and waitpid() */
	if (1 == 2)
	  printf("do for, execve and waitpid\n");
	else
	  fprintf(stderr, "%s: Command not found.\n", args[0]);
      }

      blank_args(argsct, args); /* empty argsct number of  args */
    }
  printf("exit\n");

  free(prompt);
  free(commandline);
  free(pwd);
  free(owd);
  blank_args(argsct, args);
  free(args);
  free_path(pathlist);
  return 0;
}



/****************************************************************/
/********************** Built-in commands ***********************/
/****************************************************************/
/* see header file for function descriptions */

char *which(char *command, struct pathelement *pathlist) {
  if(!pathlist) return NULL; /* null pathlist */
  char *path;
  int pathlength;
  while(pathlist != NULL) {
    path = malloc(sizeof(char)*(strlen(command)+strlen(pathlist->element)+2));
    //build a path then see if it can be accessed
    strcat(strcat(strcpy(path, pathlist->element), "/"), command);
    if(access(path, F_OK | X_OK) == 0)
      return path;
    pathlist = pathlist->next;
    free(path);
  }
  return NULL;
}

char *where(char *command, struct pathelement *pathlist) {
  if(!pathlist) return NULL; /* null pathlist */
  char *path;
  char *ret = malloc(sizeof(char)*2);
  ret = strcpy(ret, "");
  while(pathlist != NULL) {
    path = malloc(sizeof(char)*(strlen(command)+strlen(pathlist->element)+2));
    //build a path, if it can be accessed append it to the return value
    strcat(strcat(strcpy(path, pathlist->element), "/"), command);
    if(access(path, F_OK | X_OK) == 0) {
      ret = realloc(ret, sizeof(path)+sizeof(ret)+sizeof(char)*7);
      ret = strcat(strcat(ret, path), "\n");
    }
    pathlist = pathlist->next;
    free(path);
  }
  return ret;
}

void cd(char **owd, char **pwd, char *homedir) {

}

void list (char *dir) {
  DIR *d;
  struct dirent *de;
  if ((d = opendir(dir)) == NULL) {
    printf("list: cannot access %s: No such file or directory\n", dir);
    return;
  }
  de = readdir(d);
  while (de != NULL) {
    printf("%s\n", de->d_name);
    de = readdir(d);
  }
  closedir(d);
  return;
}

void printenv(char **envp) {

}



/****************************************************************/
/*********************** Helper functions ***********************/
/****************************************************************/
/* see header file for function descriptions */

int parse_command(char* commandline, char** args) {
  commandline[strlen(commandline) - 1] = '\0'; /* strip newline */
  /* saveptr for strtok_r() and delim char */
  char* saveptr;
  const char* delim = " ";
  /* temp string to hold the intermediate result from strtok_r */
  char* temp;
  
  /* extract command */
  temp = strtok_r(commandline, delim, &saveptr);

  if(temp == NULL) {
    return 0;
  } else {
    /* allocate memory for the command and put the string in it */
    args[0] = malloc(sizeof(char) * (strlen(temp) + 1));
    strcpy(args[0], temp);
    
    /* extract args */
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

void blank_args(int argsct, char **args) {
  for(int i = 0; i < argsct; i++) {
    free(args[i]);
    args[i] = NULL;
    }
}
