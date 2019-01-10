//
// @file : execute.c
// @author : 2012104208 Jongmin Yun
// @datetime : 2019-01-10 23:53
// @description : Function definition file of Split args from string chunk. 
// and forking children process and execute program.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "smsh.h"

int Execute(char** iArgv)
{
  int pid;
  int childInfo = -1;

  if (iArgv[0] == NULL) { return 0; }
  if ((pid = fork()) == -1) { perror("fork"); }
  else if (pid == 0)
  {
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    execvp(iArgv[0], iArgv);
    perror("Cannot execute command");
    exit(1);
  }
  else
  {
    if (wait(&childInfo) == -1)
    {
      perror("wait");
    }
  }

  return childInfo;
}
