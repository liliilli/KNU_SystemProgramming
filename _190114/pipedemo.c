///
/// @file : pipedemo.c
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-14 
/// @description : 
///

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

#include "HelperGlobal.h"

int main(int argc, char** argv)
{
	// Get a pipe.
	int apipe[2];
	assert.MustSuccess(pipe(apipe), "Pipe must be succeeded.");
	printf("Got a pipe. It is file descriptor id [%d, %d]\n", apipe[0], apipe[1]);

	// Read from stdin, write into pipe, read from pipe, and print.
	char buf[BUFSIZ];
	while (fgets(buf, BUFSIZ, stdin)) 
	{
		unsigned len = strlen(buf);
		if (write(apipe[1], buf, len) != len)
		{
			perror("Writing to pipe");
			break;
		}

		for (unsigned i = 0; i < len; ++i) { buf[i] = 'X'; }

		len = read(apipe[0], buf, BUFSIZ);
		if (len == -1)
		{
			perror("Reading from pipe");
			break;
		}

		if (write(1, buf, len) != len)
		{
			perror("Writing to stdout");
			break;
		}
	}

	return 0;
}

