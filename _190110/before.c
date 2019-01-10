#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	char* arg[2] = { "./after" , 0 };
	pid_t pid = getpid();

	printf("Before execvp() : %d\n", pid);
	execvp(arg[0], arg);
	return 0;
}
