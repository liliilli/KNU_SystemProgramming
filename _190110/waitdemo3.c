//
// @file : waitdemo3.c
// @author : 2012104208 Jongmin Yun
// @datetime : 2019-01-10 11:55
// @description : Create child process and wait for 5 seconds with SIGCHLD's callback handler.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "HelperMacro.h"
#include "HelperGlobal.h"

#define DELAY 5

int breakFlag = FALSE;

// @brief Just wait for `iDelay` seconds and exit with status value 14.
void ChildCode(int iDelay);

// @brief Callback function for SIGCHLD. Just wait for exiting children process.
// and get status value from that, print status value.
void ParentCode(int iSignal);

int main(int argc, char** argv)
{
	NOTUSE(argc);
	NOTUSE(argv);

	int childPid;
	printf("Before : MyPid is %d\n", getpid());

	global.BindSIGCHLD(ParentCode);
	
	childPid = fork();
	if (childPid == -1) { perror("Fork"); }
	else if(childPid == 0) 
	{ 
		ChildCode(DELAY);
	}
	else
	{
		// Wait until `breakFlag` to be TRUE.
		// In waiting, print waiting string each second.
		while (breakFlag == FALSE)
		{
			if (breakFlag == TRUE) { break; }
			sleep(1);
			printf("waiting...\n");
		}
	}
	return 0;
}

void ChildCode(int iDelay)
{
	printf("Child %d here. will sleep for %d seconds\n", getpid(), iDelay);
	sleep(iDelay);
	printf("Child done. about to exit.\n");
	exit(14);
}

void ParentCode(int iSignal)
{
	NOTUSE(iSignal);

	int waitRv;
	wait(&waitRv);

	int ret, dmp, sig;
	systm.GetWaitStatus(waitRv, &ret, &dmp, &sig);
	printf("Status : exit=%d, sig=%d, core=%d\n", ret, sig, dmp);
	breakFlag = TRUE;
}

