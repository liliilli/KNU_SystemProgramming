#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

int main()
{
	struct termios info;
	int rv; 

	rv = tcgetattr(0, &info);
	if (rv == -1)
	{
		perror("tcgetattr"); exit(1);
	}	

	if (info.c_lflag & ECHO) { printf("Echo is on"); }	
	else { printf("Echo is off"); }

	return 0;
}
