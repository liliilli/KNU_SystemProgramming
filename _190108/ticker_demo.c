#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "SetTicker.h"

void Countdown(int iSignal);

int main(int argc, char** argv)
{
	signal(SIGALRM, Countdown);
	if (SetTicker(16) == -1) { perror("SetTicker"); }
	else
	{
		while (1) { pause(); }
	}
	return 0;
}

void Countdown(int iSignal)
{
	static int num = 10;
	printf("%d ...", num--);
	fflush(stdout);

	if (num < 0) { printf("DONE!\n"); exit(0); }
}
