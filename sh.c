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
#include <sys/types.h>
#include <sys/stat.h>
#include "sh.h"

/* see header file for function descriptions */

extern pid_t cpid;

int sh( int argc, char **argv, char **envp )
{
  /* prefix to prompt, changed with 'prompt' command */
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  /* user input */
  char *commandline = calloc(MAX_CANON, sizeof(char));
  /* command = ??? */
  /*  arg = ??? */
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
  struct alias *aliaslist = NULL;
  struct history *hist = NULL;
  
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

      hist = add_history(hist, commandline);
      
      /* process the command */
      /* command holds the command to be run, args holds arguments */
      /* if argsct is -1, command is null*/
      if((argsct = parse_command(commandline, args)) == 0)
	continue;

      /* check for each built in command and implement */

      /* if command is exit with 0 args, exit */
      if(strcmp(args[0], "exit") == 0) { /****************************** exit */
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
	  if(argsct == 1)
	    cd(&owd, &pwd, homedir, NULL);
	  else
	    cd(&owd, &pwd, homedir, args[1]);
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
      } else if(strcmp(args[0], "pid") == 0) { /************************* pid */
	printf("Executing built-in command %s\n", args[0]);
	printf("[%d]\n", getpid());
      } else if(strcmp(args[0], "kill") == 0) { /*********************** kill */
	printf("Executing built-in command %s\n", args[0]);
	if(argsct < 2) {
	  printf("%s: Too few arguments.\nusage: kill [-signum] pid\n", args[0]);
	} else {
	  if(args[1][0] == '-') { /* signal specified with flag */
	    int sig = atoi(args[1]+1);
	    if((sig > 64) || (sig < 1)) {
	      printf("invalid signal flag. specify signal number between 1 and 64\n");
	    } else {
	      if(argsct < 3) {
		printf("no pid specified\n");
	      } else {
		for(i = 2; i < argsct; i++) {
		  int pid = atoi(args[i]);
		  if(kill(pid, sig) == -1)
		    printf("kill: %s: no such pid\n", args[i]);
		}
	      }
	    }
	  } else { /* just kill pid */
	    for(i = 1; i < argsct; i++) {
	      int pid = atoi(args[i]);
	      if(kill(pid, 9) == -1)
		printf("kill: %s: no such pid\n", args[i]);
	    }
	  }
	}
      } else if(strcmp(args[0], "prompt") == 0) { /******************* prompt */
	printf("Executing built-in command %s\n", args[0]);
	if (argsct == 1) { /* no args */
	  printf("input prompt prefix: ");
	  fgets(prompt, PROMPTMAX, stdin);
	  prompt[strlen(prompt)-1] = '\0';
	} else
	  strcpy(prompt, args[1]);
      } else if(strcmp(args[0], "printenv") == 0) { /*************** printenv */
	printf("Executing built-in command %s\n", args[0]);
	//printenv();
	if (argsct == 1) {
	  char **env;
	  for (env = envp; *env != 0; env++)
	    printf("%s\n", *env);
	} else {
	  char *env;
	  int i = 1;
	  while((i < argsct) && ((env = getenv(args[i])) != NULL)) {
	    printf("%s\n", env);
	    i++;
	  }
	}
      } else if(strcmp(args[0], "alias") == 0) { /********************* alias */
	printf("Executing built-in command %s\n", args[0]);
	switch(argsct) {
	case 1:
	  print_alias(aliaslist, NULL);
	  break;
	case 2:
	  print_alias(aliaslist, args[1]);
	  break;
	case 3:
	  aliaslist = set_alias(aliaslist, args[1], args[2]);
	  break;
	default:
	  printf("Too many args. Use one or no arguments to print aliases\n");
	  printf("or use 2 arguments to set a new alias\n");
	}
      } else if(strcmp(args[0], "history") == 0) { /***************** history */
	printf("Executing built-in command %s\n", args[0]);
	switch(argsct) {
	case 1:
	  print_history(hist, 10);
	  break;
	case 2:
	  print_history(hist, atoi(args[1]));
	  break;
	default:
	  printf("%s: too many arguments\n", args[0]);
	}
      } else if(strcmp(args[0], "setenv") == 0) { /******************* setenv */
	printf("Executing built-in command %s\n", args[0]);
	char **env;
	switch(argsct) {
	case 1: /* no args, do printenv */
	  for (env = envp; *env != 0; env++)
	    printf("%s\n", *env);
	  break;
	case 2: /* one arg - add empty env variable */
	  setenv(args[1], "", 1);
	  /* special case for home and path */
	  if (strcmp(args[1], "PATH") == 0) {
	    free_path(pathlist);
	    pathlist = get_path();
	  } else if(strcmp(args[1], "HOME")) {
	    homedir = strcpy(homedir, "/");
	  }
	  break;
	case 3: /* two args - add new env variable */
	  setenv(args[2], args[3], 1);
	  /* special case for home and path */
	  if (strcmp(args[1], "PATH") == 0) {
	    free_path(pathlist);
	    pathlist = get_path(); // update path list
	  } else if(strcmp(args[1], "HOME")) {
	    homedir = strcpy(homedir, args[1]);
	  }
	  break;
	default:
	  printf("setenv: Too many arguments.\n");
	}
      }
      /****************************************************************/
      /****************************************************************/
      /****************************************************************/
      /*  else  program to exec */
      /* find it */
      /* do fork(), execve() and waitpid() */
      else {
	if(is_absolute(args[0]) == 0) { /* file is absolute */
	  if(!access(args[0], F_OK)) { /* file exists */
	    if(!access(args[0], X_OK)) { /* usr has exec perms */
	      /* do fork(), execve(), and waitpid() */
	      struct stat status;
	      stat(args[0], &status);
	      if(S_ISREG(status.st_mode)) {
		cpid = fork();
		if(cpid == 0) {
		  printf("Executing %s\n", args[0]);
		  if(execve(args[0], &args[0], envp) == -1) {
		    printf("%s: Command not found.\n", args[0]);
		  }
		} else if(cpid > 0) {
		  int status;
		  waitpid(cpid, &status, 0);
		  if(WEXITSTATUS(status) != 0) {
		    printf("%s: Command exited with status: %d\n", args[0], WEXITSTATUS(status));
		  }
		} else { /* there was a forking issue */
		  printf("%s: Unable to fork child process.\n", args[0]);
		}
		cpid = 0; /* reset child process id*/
	      } else { /* file is directory */
		fprintf(stderr, "%s: Cannot execute a directory.\n", args[0]);
	      }
	    } else { /* usr does not have perms */
	      fprintf(stderr, "%s: Permission Denied.\n", args[0]);
	    }
	  } else { /* file does not exist */
	    fprintf(stderr, "%s: Command not found.\n", args[0]);
	  }
	} else { /* is not absolute */
	  command = which(args[0], pathlist);
	  if(command == NULL) {
	    printf("%s: Command not found.\n", command);
	  } else {
	    /* do fork(), execve(), and waitpid() */
	    cpid = fork();
	    if(cpid == 0) {
	      printf("Executing external command %s\n", command);
	      if(execve(command, &args[0], envp) == -1) {
		printf("%s: Command not found.\n", command);
	      }
	    } else if(cpid > 0) {
	      int status;
	      waitpid(cpid, &status, 0);
	      if(WEXITSTATUS(status) != 0) {
		printf("%s: Command exited with status: %d\n", command, WEXITSTATUS(status));
	      }
	    } else {
	      printf("%s: Unable to fork child process.\n", command);
	    }
	    cpid = 0; /* reset child process id */
	  }
	  free(command);
	}
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
  free_alias(aliaslist);
  free_history(hist);
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
    /* build a path then see if it can be accessed */
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
    /* build a path, if it can be accessed append it to the return value */
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

void cd(char **owd, char **pwd, char *homedir, char *arg) {

  if((arg == NULL) || (strcmp(arg, "~") == 0)) { /* go home */
    strcpy(*pwd, *owd);
    strcpy(*owd, homedir);
    chdir(homedir);
    return;
  } else if(strcmp(arg, "-") == 0) { /* go back */
    if(chdir(*pwd) == 0) {
      char *tempdir = malloc(sizeof(char)*(strlen(*owd)+1));
      tempdir = strcpy(tempdir, *owd);
      strcpy(tempdir, *owd);
      strcpy(*owd, *pwd);
      strcpy(*pwd, tempdir);
      free(tempdir);
      return;
    }
  } else if(chdir(arg) == 0){ /* go to path if valid */
    strcpy(*pwd, *owd);
    getcwd(*owd, PATH_MAX+1);
    return;
  } else {
    printf("%s: no such directory\n", arg);
    return;
  }
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

int is_absolute(char *command) {
  /* if path starts at root or is a dot-slash path, return 0 */
  if((command[0] == '/') ||
     ((command[0] == '.') && (command[1] == '/'))) {
    return 0;
  } else
    return 1;
}
