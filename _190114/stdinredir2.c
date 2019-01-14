///
/// @file : stdinredir2.c
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-14 
/// @description : 
///
/// To use DUP2 instead of DUP, build it with -D FLAG_DUP2
///

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char** argv)
{
	char line[100];

	// Read and print three lines.
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);

	// Redirect input.
	int fd = open("/etc/passwd", O_RDONLY);
#ifdef FLAG_DUP2
	int fdRedirect = dup2(fd, 0);
#else
	assert(fd > 0);
	close(0);
	int fdRedirect = dup(fd);
#endif
	assert(fdRedirect == 0);

	// read and print three lines.
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	return 0;
}

