//
// @file : controlflow.c
// @author : 2012104208 Jongmin Yun
// @datetime : 2019-01-11 10:08
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

int SynError(const char* iMsg);

// @enum EStatus (states)
// @brief 
enum EStatus
{
	EStat_Neutral,
	EStat_WantThen,
	EStat_ThenBlock
};

// @enum EResult
// @brief
enum EResult
{
	ERes_Success,
	ERes_Failure
};

static int sIfStatus   = EStat_Neutral;
static int sIfResult   = ERes_Success;
static int sLastStatus = 0;

int OkToExecute()
{
	if (sIfStatus == EStat_WantThen)
	{
		SynError("then expected but not found");
		return 0;
	}

	if (sIfStatus == EStat_ThenBlock && sIfResult == ERes_Success) { return 1; }
	if (sIfStatus == EStat_ThenBlock && sIfResult == ERes_Failure) { return 0; }
	return 1;
}

int IsControlCommand(char* s)
{
	return (strcmp(s, "if") == 0 || strcmp(s, "then") == 0 || strcmp(s, "fi") == 0);
}

int DoControlCommand(char** iArgv)
{
	char* cmd = iArgv[0];

	if (strcmp(cmd, "if") == 0)
	{
		if (sIfStatus != EStat_Neutral) { return SynError("Unexpected `if` found"); }
		else
		{
			sLastStatus = Process(iArgv + 1);
			sIfResult = (sLastStatus == 0 ? ERes_Success : ERes_Failure);
			sIfStatus = EStat_WantThen;
			return 0;
		}
	}
	else if (strcmp(cmd, "then") == 0)
	{
		if (sIfStatus != EStat_WantThen) { return SynError("Unexpected `then` found"); }
		else
		{
			sIfStatus = EStat_ThenBlock;
			return 0;
		}
	}
	else if (strcmp(cmd, "fi") == 0)
	{
		if (sIfStatus != EStat_ThenBlock) { return SynError("Unexpected `fi` found"); }
		else
		{
			sIfStatus = EStat_Neutral;
			return 0;
		}
	}
	else { assert.Fatal("Internal error processing", cmd, 2); }
	return -1;
}

int SynError(const char* iMsg)
{
	sIfStatus = EStat_Neutral;
	fprintf(stderr, "Syntax Error : %s\n", iMsg);
	return -1;
}


