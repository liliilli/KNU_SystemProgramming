///
/// @file : stdinredir1.c
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-14 
/// @description : 
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
	close(0);
	int fd = open("/etc/passwd", O_RDONLY);
	assert(fd == 0);

	// read and print three lines.
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);

	
}

