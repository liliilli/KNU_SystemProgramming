///
/// @file : tinybc.c
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-15 
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

// @brief Close pipes which are not used in subroutine.
void ClosePipes(int* iPipeList, unsigned iSize)
{
	for (unsigned i = 0; i < iSize; ++i) 
	{ 
		close(iPipeList[i]); 
	}
}

void ProcessDc(void* iArg)
{
#ifndef NDEBUG
	puts("ProcessDc");
#endif
	struct Arg* arg = (struct Arg*)(iArg);
	ClosePipes(arg->ptrClose, arg->ptrCloseSize);

	assert.MustSuccess(dup2(arg->input, 0), "Dc : Could not redirect stdin");
	assert.MustSuccess(dup2(arg->output, 1), "Dc : Could not redirect stdout");
	close(arg->input);
	close(arg->output);

	execlp("dc", "dc", "-", NULL);
	assert.NeverReachThisLine("Could not run `dc`");
}

void ProcessBc(void* iArg)
{
#ifndef NDEBUG
	puts("ProcessBc");
#endif
	// Setup
	struct Arg* arg = (struct Arg*)(iArg);
	ClosePipes(arg->ptrClose, arg->ptrCloseSize);

	FILE* fpOutput = fdopen(arg->output, "w");
	FILE* fpInput  = fdopen(arg->input, "r");
	M_ASSERT(fpOutput != NULL && fpInput != NULL, "Error converting pipes to streams.");

	char message[BUFSIZ];
	char operation[BUFSIZ];
	int lOperand;
	int rOperand;

	// Main loop
	while (1)
	{
		// Read
		printf("tinybc : ");
		if (fgets(message, BUFSIZ, stdin) == NULL) { break; }

		// Parse & Evalulation
		if (sscanf(message, "%d%[-+*/^]%d", &lOperand, operation, &rOperand) != 3)
		{
			fprintf(stderr, "Syntax error\n");
			continue;
		}

		// Parse & Evaluation (Send query to dc (child))
		static const char* kQueryString = "%d\n\
																			 %d\n\
																			 %c\n\
																			 p\n";
		if (fprintf(fpOutput, kQueryString, lOperand, rOperand, *operation) == EOF)
		{
			assert.Fatal("Error", "Failed to query to dc for getting calculation result.", 1);
		}

		// Print
		fflush(fpOutput);
		if (fgets(message, BUFSIZ, fpInput) == NULL) { break; }
		printf("%d %c %d = %s", lOperand, *operation, rOperand, message);
	}

	fclose(fpOutput);
	fclose(fpInput);
	close(arg->input);
	close(arg->output);
}

int main(int argc, char** argv)
{
	// Get a pipe.
	int toDc[2];	// [Read, Write]
	int fromDc[2];// [Read, Write]
	assert.MustSuccess(pipe(toDc), "Pipe must be succeeded.");
	assert.MustSuccess(pipe(fromDc), "Pipe must be succeeded.");

	int childKillPipe[] = {toDc[1], fromDc[0]};
	struct Arg toChild = {toDc[0], fromDc[1], M_INPUT_ARRAY(childKillPipe), ""};

	int parentKillPipe[] = {toDc[0], fromDc[1]};
	struct Arg toParent = {fromDc[0], toDc[1], M_INPUT_ARRAY(parentKillPipe), ""};

	systm.ForkChildExt(ProcessDc, &toChild, ProcessBc, &toParent);
	wait(NULL);
	return 0;
}

