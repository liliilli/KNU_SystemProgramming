#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "HelperMacro.h"
#include "HelperGlobal.h"

#define DELAY 5

void ChildCode(int iDelay);
void ParentCode(int iChildPid);

int main(int argc, char** argv)
{
	int childPid;
	printf("Before : MyPid is %d\n", getpid());
	
	childPid = fork();
	if (childPid == -1) { perror("Fork"); }
	else if(childPid == 0) 
	{ 
		ChildCode(DELAY);
	}
	else
	{
		ParentCode(childPid);
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

void ParentCode(int iChildPid)
{
	int waitRv;
	pid_t exitChildPid = wait(&waitRv);
	if (exitChildPid == iChildPid)
	{
		printf("done waiting for %d. Wait returned : %d\n", iChildPid, waitRv);

		int ret, dmp, sig;
		systm.GetWaitStatus(waitRv, &ret, &dmp, &sig);
		printf("Status : exit=%d, sig=%d, core=%d\n", ret, sig, dmp);
	}
	else
	{
		printf("Not matched child pid. expected : %d but returned : %d.\n", iChildPid, exitChildPid);
	}
}

