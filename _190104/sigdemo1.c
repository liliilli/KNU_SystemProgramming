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

	void f(int);
	signal(SIGINT, f);
	for (int i = 0; i < 5; ++i)
	{
		printf("Hello\n");
		sleep(1);
	}
	return 0;
}

void f(int signum)
{
	(void)signum;
	printf("OUCH\n");
}
