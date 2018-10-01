#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* linked list for aliases */
struct alias {
  char *command; /* the command this alias replaces */
  char *alias; /* the replacement command */
  struct alias *next; /* pointer to next alias */
};

/* with argsct = 1, prints all aliases, otherwise it
goes through argct - 1 number of args and prints out
alilases for the given command if any exist */
void print_alias(struct alias *list, char *arg);
/* sets a new alias in the list, returns pointer to head */
struct alias* set_alias(struct alias *list, char *command, char *alias);
/* returns a char* of the alias if alias found, NULL otherwise */
char* get_alias(struct alias *list, char *command);
/* frees the alias table */
void free_alias(struct alias *list);
