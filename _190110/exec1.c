#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	char* arglist[3] = { "ls", "-l", 0 };
	printf("* * * About to exec ls -l\n");
	execvp(arglist[0], arglist);
	printf("* * * ls is done. bye\n");
	return 0;
}
