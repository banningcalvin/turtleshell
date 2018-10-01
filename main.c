#include "sh.h"
#include <signal.h>
#include <stdio.h>

void sig_handler(int signal);

pid_t cpid = 0;

int main( int argc, char **argv, char **envp )
{
  signal(SIGINT, sig_handler);
  signal(SIGTSTP, sig_handler);
  return sh(argc, argv, envp);
}

void sig_handler(int signal)
{
  switch(signal) {
  case SIGINT:
    if (cpid != 0) {
      kill(cpid, SIGINT);
      printf("\n");
    }
    break;
  case SIGTSTP:
    break;
  }
}
