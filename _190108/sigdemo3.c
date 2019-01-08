#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void IntHandler(int);
void QuitHandler(int);

int main(int argc, char** argv)
{
	signal(SIGINT, IntHandler);
	signal(SIGQUIT, QuitHandler);

	int nchars;
	char input[100];

	do {
		printf("\nType a message\n");
		nchars = read(0, input, 100 - 1);
		if (nchars == -1)
		{
			perror("read returned an error.");
		}
		else
		{
			input[nchars] = '\0';
			printf("You typed : %s\n", input);
		}
	}
	while (strncmp(input, "quit", 4) != 0);
}

void IntHandler(int iSignal)
{
	printf("Received signal %d... waiting\n", iSignal);
	sleep(2);
	printf("Leaving IntHandler\n");
}

void QuitHandler(int iSignal)
{
	printf("Received signal %d... waiting\n", iSignal);
	sleep(3);
	printf("Leaving QuitHandler\n");
}
