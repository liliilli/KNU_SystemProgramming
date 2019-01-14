///
/// @file : pipe2.c
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-14 
/// @description : 
/// Show result with 3 successive program pipelining.
///
/// Usage : pipe2 program1 program2 program3
/// Example ) pipe2 who sort head
///

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

#include "HelperGlobal.h"

#define M_INPUT_ARRAY(__Array__) __Array__, (sizeof(__Array__) / sizeof(__Array__[0]))

struct Arg
{
	// @brief Specifies input pipe instead of default `stdin`.
	// If value is given as -1, it passes replacing `stdin`.
	int input;
	// @brief Specifies output pipe instead of default `stdout`.
	// If value is given as -1, it passes replacing `stdout`. 
	int output;
	// @brief The array to close other pipes which are not used in each procedure.
	// `ptrCloseSize` must be specified correctly.
	int* ptrClose;
	unsigned ptrCloseSize;
	// @brief Program name to execute.
	char* program;
};

void ProcessProgram(void* iArg)
{
	struct Arg* arg = (struct Arg*)iArg;

	// Close pipes which are not used in subroutine.
	for (unsigned i = 0; i < arg->ptrCloseSize; ++i) { close(arg->ptrClose[i]); }

	if (arg->input != -1)
	{ // If value is not -1, it replace `stdin` with `arg->input`.
		assert.MustSuccess(dup2(arg->input, 0), "Could not redirect stdin");
		close(arg->input);
	}
	if (arg->output != -1)
	{ // If value is not -1, it replace `stdout` with `arg->input`.
		assert.MustSuccess(dup2(arg->output, 1), "Could not redirect stdout");
		close(arg->output);
	}

	// Execute program!.
	char* vec[] = {arg->program, NULL};
	systm.ExecuteProgram(arg->program, vec, M_NOTCHILD);
	assert.NeverReachThisLine("Must not reach this code line");
}

int main(int argc, char** argv)
{
	M_ASSERT(argc == 4, "Usage : pipe2 cmd1 cmd2 cmd3\n");

	// Get a pipe.
	int apipe[2]; // 3, 4
	int bpipe[2]; // 5, 6
	assert.MustSuccess(pipe(apipe), "Pipe must be succeeded.");
	assert.MustSuccess(pipe(bpipe), "Pipe must be succeeded.");

	int bChild = systm.ForkChild();
	M_ASSERT(bChild != -1, "Must be succeeded");

	// PROCEDURE
	//   aChild will process program with `stdin` and output pipe `apipe[1]`.
	//   Afterward aChild, bChild get buffer from `apipe[0]` connected to `apipe[1]`,
	// do process program with `apipe[0]` input pipe and output pipe `bpipe[1]`.
	//   At final, parent process get buffer from `bpipe[0]` connected to `bpipe[1]`,
	// do process program with `bpipe[1]` input pipe and `stdout`.

	if (bChild == 0)
	{
		int aChild = systm.ForkChild();
		M_ASSERT(aChild != -1, "Must be succeeded");

		if (aChild == 0)
		{ // Get stdin, Write result to pipe 4.
			int childClose[] = {apipe[0], bpipe[0], bpipe[1]};
			struct Arg child = {-1, apipe[1], M_INPUT_ARRAY(childClose), argv[1]};
			ProcessProgram(&child);
		}
		else
		{ // Get buffer from pipe 3, Write result to pipe 6.
			int childClose[] = {apipe[1], bpipe[0]};
			struct Arg child = {apipe[0], bpipe[1], M_INPUT_ARRAY(childClose), argv[1]};
			ProcessProgram(&child);
		}
	}
	else
	{ // Get buffer from pipe 5, Write result to stdout.
		int parentClose[] = {apipe[0], apipe[1], bpipe[1]};
		struct Arg parent = {bpipe[0], -1, M_INPUT_ARRAY(parentClose), argv[2]};
		ProcessProgram(&parent);
	}

	return 0;
}

