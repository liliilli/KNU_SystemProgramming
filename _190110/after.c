#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	pid_t pid = getpid();

	printf("After execvp() : %d\n", pid);
	return 0;
}
