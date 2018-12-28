#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
	struct stat infoBuf;

	if (argc != 2)
	{ return 0; }

	if (stat(argv[1], &infoBuf) == -1)
	{
		perror(argv[1]);
		return 0;
	}

	printf("The size of \"%s\" is %ld\n", argv[1], infoBuf.st_size);
	return 0;
}
