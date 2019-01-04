#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

#define SAVE_CURRENT_MODE 0
#define RESTORE_MODE 1

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	signal(SIGINT, SIG_IGN);
	puts("You can't stop me!");

	for (;;)
	{
		printf("Hello\n");
		sleep(1);
	}
	return 0;
}
