#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void IntHandler(int iSignal);

int main(int argc, char** argv)
{
	struct sigaction newHandler;
	sigset_t blocked;
	char x[100];

	newHandler.sa_handler = IntHandler;
	newHandler.sa_flags = SA_RESTART | SA_NODEFER;

	sigemptyset(&blocked);
	sigaddset(&blocked, SIGQUIT);
	newHandler.sa_mask = blocked;

	if (sigaction(SIGINT, &newHandler, NULL) == -1) { perror("SigAction"); }
	else
	{
		while (1)
		{
			fgets(x, 100, stdin);
			printf("input : %s", x);
		}
	}
}

void IntHandler(int iSignal)
{
	printf("Called with signal %d\n", iSignal);
	sleep(iSignal);
	printf("Done handling signal %d\n", iSignal);
}
