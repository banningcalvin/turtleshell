#include "history.h"

void print_history(struct history *hist, int num) {
  struct history *tmp = hist;
  for(int i = 0; i < num; i++) {
    if(tmp == NULL) {
      printf("history: no more elements in history\n");
      break;
    }
    printf("%s", tmp->command);
    tmp = tmp->next;
  }
}

struct history *add_history(struct history *hist, char *command) {
  struct history *new_element = malloc(sizeof(struct history));
  new_element->command = malloc((strlen(command)+1)*sizeof(char));
  strcpy(new_element->command, command);
  new_element->next = NULL;
  if(hist == NULL) { /* this is the first element */
    hist = new_element;
    return new_element;
  } else { /* add this to the beginning */
    new_element->next = hist;
    return new_element;
  }
}

void free_history(struct history *hist) {
  struct history *next;
  while(hist != NULL) {
    next = hist->next;
    free(hist->command);
    free(hist);
    hist = next;
  }
}
