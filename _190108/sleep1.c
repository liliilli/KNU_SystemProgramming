#include <stdio.h>

#include <unistd.h>
#include <signal.h>

void WakeUp(int);

int main(int argc, char** argv)
{
	printf("About to sleep for 4 seconds\n");
	signal(SIGALRM, WakeUp);
	alarm(4);
	pause();
	printf("Morning so soon?\n");
}

void WakeUp(int iSignum)
{
#ifndef SHHHH
	printf("Alarm received from kernel.\n");
#endif
}
