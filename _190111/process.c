//
// @file : process.c
// @author : 2012104208 Jongmin Yun
// @datetime : 2019-01-11 10:40
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
#include "HelperGlobal.h"

int Process(char** iArgv)
{
	if (iArgv[0] == NULL) { return 0; }
	else if (IsControlCommand(iArgv[0]) == 1) { return DoControlCommand(iArgv); }
	else if (OkToExecute() == 1) { return Execute(iArgv); }

  return 0;
}
