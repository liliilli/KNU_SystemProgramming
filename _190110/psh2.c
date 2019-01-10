#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "HelperMacro.h"
#include "HelperGlobal.h"

int Execute(char* arglist[])
{
	int childPid = systm.ExecuteProgram(arglist[0], arglist, M_CHILD);
	int exitStatus;
	while (wait(&exitStatus) != childPid)
		;

	int ret, dmp, sig;
	systm.GetWaitStatus(exitStatus, &ret, &dmp, &sig);
	printf("Child exited with status : exit=%d, sig=%d, core=%d\n", ret, sig, dmp);
}

char* HeapMakeString(char* buf)
{
	buf[strlen(buf) - 1] = '\0';
	char* cp = (char*)malloc(strlen(buf) + 1);
	if (cp == NULL)
	{
		fprintf(stderr, "No memory\n");
		exit(1);
	}

	strcpy(cp, buf);
	return cp;
}

int main(int argc, char** argv)
{
	NOTUSE(argc);
	NOTUSE(argv);

	char* arglist[21] = { '\0', };
	int numargs = 0;
	char argbuf[100];

	while (numargs < 20)
	{
		printf("Arg[%d]? ", numargs);
		if (fgets(argbuf, sizeof(argbuf) / sizeof(argbuf[0]), stdin)
		&&  argbuf[0] != '\n')
		{
			arglist[numargs] = HeapMakeString(argbuf); 
			numargs += 1;
		}
		else if (numargs > 0)
		{
			Execute(arglist);
			numargs = 0;
		}
	}
	return 0;
}
