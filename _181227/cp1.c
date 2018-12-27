#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096
#define COPYMODE    0644

enum EErrorCode
{
	ErrFailedToRead = -1
};

void ProcessError(char* iMsg, const char* iFilePath);

int main(int argc, char* argv[])
{
	int fdIn, fdOut, numChars;
	char buffer[BUFFER_SIZE];

	if (argc != 3)
	{ /* Check argc is 3 (program source_file destination_file) */
		fprintf(stderr, "usage : %s source destination\n", argv[0]);
		exit(1);
	}

	if ((fdIn = open(argv[1], O_RDONLY)) == ErrFailedToRead)
	{
		ProcessError("Failed to open.\n", argv[1]);
	}

	if ((fdOut = creat(argv[2], COPYMODE)) == ErrFailedToRead)
	{
		ProcessError("Failed to open.\n", argv[2]);
	}

	while ((numChars = read(fdIn, buffer, BUFFER_SIZE)) > 0)
	{
		if (write(fdOut, buffer, numChars) != numChars)
		{
			ProcessError("Write error to ", argv[2]);
		}

		if (numChars == ErrFailedToRead)
		{
			ProcessError("Read error from ", argv[1]);
		}
	}

	if (close(fdIn) == -1 || close(fdOut) == -1)
	{
		ProcessError("Error closing files", "");
	}
	return 0;
}

void ProcessError(char* iMsg, const char* iFilePath)
{
	fprintf(stderr, "[Error] : %s", iMsg);
	perror(iFilePath);
	exit(1);
}

