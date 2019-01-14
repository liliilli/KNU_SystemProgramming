///
/// @file : listargs.c
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-14 
/// @description : Test file for printing and communcation of stdout(1) stdin(0) stderr(2).
/// file descriptors for tty.
///

#include <stdio.h>

int main(int argc, char** argv)
{
	printf("Number of args : %d, Args are : \n", argc);
	for (int i = 0; i < argc; ++i)
	{
		printf("args [%d] %s\n", i, argv[i]);
	}

	fprintf(stderr, "This message is sent to `stderr`.\n");
	return 0;
}
