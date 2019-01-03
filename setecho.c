#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

int main(int argc, char** argv)
{
	struct termios info;
	int rv; 

	if (argc == 1) { exit(0); }

	rv = tcgetattr(0, &info);
	if (rv == -1)
	{
		perror("tcgetattr"); exit(1);
	}	

	if (argv[1][0] == 'y') { info.c_lflag |= ECHO; }
	else { info.c_lflag &= ~ECHO; }

	if (tcsetattr(0, TCSANOW, &info) == -1)
	{
		perror("tcsetattr"); exit(1);
	}	
	return 0;
}
