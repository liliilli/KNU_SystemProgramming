//
// @file : smsh1.c
// @author : 2012104208 Jongmin Yun
// @datetime : 2019-01-10 22:22
// @description : Simple small-shell version 1.
// To build program, just type `build all`.
//
// First really useful version after prompting shell.
// This one prases the command line into strings use fork, exec, wait, 
// and ignores signals. 
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "HelperMacro.h"
#include "HelperGlobal.h"
#include "smsh.h"

char* sStrPrompt = "> ";

int Setup();

int main(int argc, char** argv)
{
  char* cmdLine;
  char* prompt;
  char** argList;
  int result;

  NOTUSE(argc);
  NOTUSE(argv);

  prompt = sStrPrompt;
  Setup();

  while ((cmdLine = NextCmd(prompt, stdin)) != NULL)
  {
    if ((argList = SplitLine(cmdLine)) != NULL)
    {
      result = Execute(argList);
      FreeList(argList); 
    }
    free(cmdLine);
  }
  return 0;
}

int Setup()
{
  global.BindSIGINT(SIG_IGN);
  global.BindSIGQUIT(SIG_IGN);
}

