#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	int fd;
	char buffer[512];

	/* Check args */
	if (argc != 2) 
	{
		fprintf(stderr, "Usage : Write0 ttyname\n");
		exit(1);
	}	

	/* Open device */
	fd = open(argv[1], O_WRONLY);
	if (fd == -1)
	{
		perror(argv[1]); 
		exit(1);
	}

	/* Loop until EOF on input */
	while (fgets(buffer, 512, stdin) != NULL)
	{
		if (write(fd, buffer, strlen(buffer)) == -1) { break; }
	}

	close(fd);
}
