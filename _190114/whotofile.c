///
/// @file : whotofile.c
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-14 
/// @description : 
///

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

#include "HelperGlobal.h"

int main(int argc, char** argv)
{
	int pidChild = systm.ForkChild();
	M_ASSERT(pidChild >= 0, "Fork child must be succeeded.");

	if (pidChild == 0)
	{
		int writeFile = creat("userlist", 0644);
		int fd = dup2(writeFile, 1);
		M_ASSERT(fd == 1, "writeFile's fd index is not 1(stdout).");

		char* list[] = {"who", NULL};
		systm.ExecuteProgram("who", list, M_NOTCHILD); 
		assert.NeverReachThisLine("Must never reach this code line.");
	}
	else
	{
		wait(NULL);
		printf("Done running who. Result in userlist\n");
	}

	return 0;
}

