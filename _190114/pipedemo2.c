///
/// @file : pipedemo2.c
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-14 
/// @description : 
///

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

#include "HelperGlobal.h"

const char* kChildMess = "I want a cookie.\n";
const char* kParMess = "Testing...";

void CbChild(void* iPtrApipe)
{
	int len = strlen(kChildMess);
	int* ptrApipe = (int*)iPtrApipe;
	while (1)
	{
		if (write(ptrApipe[1], kChildMess, len) != len)
		{
			assert.Fatal("Error", "Unexpected error occurred in child process", 3);
		}
		sleep(5);
	}
}

void CbParent(void* iPtrApipe)
{
	int len = strlen(kParMess);
	int* ptrApipe = (int*)iPtrApipe;
	char buf[BUFSIZ];

	while (1)
	{
		if (write(ptrApipe[1], kParMess, len) != len)
		{
			assert.Fatal("Error", "Unexpected error occurred in parent process", 4);
		}
		sleep(1);

		int readLen = read(ptrApipe[0], buf, BUFSIZ);
		if (readLen <= 0) { break; }

		write(1, buf, readLen);
	}
}

int main(int argc, char** argv)
{
	// Get a pipe.
	int apipe[2];
	assert.MustSuccess(pipe(apipe), "Pipe must be succeeded.");
	systm.ForkChildExt(CbChild, apipe, CbParent, apipe);

	return 0;
}

