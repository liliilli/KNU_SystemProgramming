#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int retFromFork, myPid;
	myPid = getpid();
	printf("Before : my pid is %d\n", myPid);

	retFromFork = fork();
	sleep(1);

	printf("After : my pid %d, fork() said %d\n",
			getpid(),
			retFromFork);

	return 0;
}
