#include "alias.h"

void print_alias(struct alias *list, char *arg) {
  if(arg == NULL) { /* print all */
    struct alias *tmp = list;
    while (tmp != NULL) {
      printf("%s='%s'\n", tmp->command, tmp->alias);
      tmp = tmp->next;
    }
  } else { /* print arg */
    struct alias *tmp = list;
    while (tmp != NULL) {
      if(strcmp(tmp->command, arg) == 0)
	printf("%s='%s'\n", tmp->command, tmp->alias);
      tmp = tmp->next;
    }
  }
}

struct alias* set_alias(struct alias *list, char *command, char *alias) {
  struct alias *tmp = list;
  struct alias *new_alias = malloc(sizeof(struct alias));
  new_alias->command = malloc((strlen(command)+1)*sizeof(char));
  new_alias->alias = malloc((strlen(alias)+1)*sizeof(char));
  strcpy(new_alias->command, command);
  strcpy(new_alias->alias, alias);
  new_alias->next = NULL;
  if (list == NULL) { /* empty list */
    list = new_alias;
  } else {
    while (tmp->next != NULL) {
      if (strcmp(tmp->alias, alias) == 0) { /* already exists */
        free(tmp->command);
        tmp->command = new_alias->command;
        free(new_alias->alias);
        free(new_alias);
        return list;
      }
      tmp = tmp->next;
    }
    if (strcmp(tmp->alias, alias) == 0) {
      free(tmp->command);
      tmp->command = new_alias->command;
      free(new_alias->alias);
      free(new_alias);
      return list;
    }
    tmp->next = new_alias;
  }
  return list;
}

char* get_alias(struct alias *list, char *command) {
  struct tmp = list;
  while(tmp != NULL) {
    if(strcmp(tmp->command, command) == 0)
      return tmp->alias;
    tmp = tmp->next;
  }
  return NULL;
}

void free_alias(struct alias *list) {
  struct alias *next;
  while(list != NULL) {
    next = list->next;
    free(list->command);
    free(list->alias);
    free(list);
    list = next;
  }
}
