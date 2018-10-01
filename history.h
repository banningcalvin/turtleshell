#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* linked list for history */
struct history {
  char *command; /* the command */
  struct history *next; /* pointer to next alias */
};

/* prints last num elements */
void print_history(struct history *hist, int num);
/* sets a new alias in the list, returns pointer to head */
struct history *add_history(struct history *hist, char *command);
/* frees the linked list */
void free_history(struct history *hist);
