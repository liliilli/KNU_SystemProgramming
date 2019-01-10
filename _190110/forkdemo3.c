#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int retFromFork, myPid;
	myPid = getpid();
	printf("Before : my pid is %d\n", myPid);

	retFromFork = fork();
	if (retFromFork == -1)
	{
		perror("fork");
	}
	sleep(0);

	if (retFromFork == 0)
	{
		printf("This process is child. my pid %d\n", getpid());
	}
	else
	{
		printf("This process is parent. my pid %d, child process id is %d\n",
				getpid(),
				retFromFork);
	}

	return 0;
}
