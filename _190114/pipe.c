///
/// @file : pipe.c
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

struct ParentArg
{
	int* pipe;
	char** argv;
};

void CbChild(void* iArg)
{
	struct ParentArg* arg = (struct ParentArg*)iArg;
	int*		apipe  = arg->pipe;
	char**	argv	 = arg->argv;

	close(apipe[0]);

	assert.MustSuccess(dup2(apipe[1], 1), "Could not redirect stdout");
	close(apipe[1]);

	char* vec[] = {argv[1], NULL};
	systm.ExecuteProgram(argv[1], vec, M_NOTCHILD);
	assert.NeverReachThisLine("Must not reach this code line");
}

void CbParent(void* iArg)
{
	struct ParentArg* arg = (struct ParentArg*)iArg;
	int*		apipe  = arg->pipe;
	char**	argv	 = arg->argv;

	close(apipe[1]);

	assert.MustSuccess(dup2(apipe[0], 0), "Could not redirect stdin");
	close(apipe[0]);

	char* vec[] = {argv[2], NULL};
	systm.ExecuteProgram(argv[2], vec, M_NOTCHILD);
	assert.NeverReachThisLine("Must not reach this code line");
}

int main(int argc, char** argv)
{
	// Get a pipe.
	M_ASSERT(argc == 3, "Usage : pipe1 cmd1 cmd2\n");

	int apipe[2];
	assert.MustSuccess(pipe(apipe), "Pipe must be succeeded.");

	struct ParentArg parentArg = {apipe, argv};
	systm.ForkChildExt(CbChild, &parentArg, CbParent, &parentArg);
	return 0;
}

