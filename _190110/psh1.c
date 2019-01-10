#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "HelperMacro.h"

int Execute(char* arglist[])
{
	if (execvp(arglist[0], arglist) == -1)
	{
		perror("execvp failed");
		exit(1);
	}
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
