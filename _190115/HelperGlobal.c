///
/// @file : HelperGlobal.cc
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-09 21:02
/// @description : Global function API container file
///

#include "HelperGlobal.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>

void global__SetTcProperty(int iSave)
{
	static struct termios globalTc; 
	static int tcFlag;

	if (iSave > 0)
	{
		tcgetattr(0, &globalTc);
		tcFlag = fcntl(0, F_GETFL);
	}
	else
	{
		tcsetattr(0, TCSANOW, &globalTc);
		fcntl(0, F_SETFL, tcFlag);
	}
}

void global__BindSIGINT(CbSignal cb)
{
	signal(SIGINT, cb); 
}

void global__BindSIGQUIT(CbSignal cb)
{
	signal(SIGINT, cb); 
}

void global__BindSIGALRM(CbSignal cb)
{
	signal(SIGALRM, cb); 
}

void global__BindSIGCHLD(CbSignal cb)
{
	signal(SIGCHLD, cb);
}

void global__BindSIGIO(CbSignal cb)
{
	signal(SIGIO, cb);
}

void global__SetEcho(int enable)
{
	struct termios tc;
	tcgetattr(0, &tc);

	if (enable <= 0) { tc.c_lflag &= ~ECHO; }
	else { tc.c_lflag |= ECHO; }

	tcsetattr(0, TCSANOW, &tc);
}

void global__SetCanonical(int enable)
{
	struct termios tc;
	tcgetattr(0, &tc);

	if (enable <= 0) { tc.c_lflag &= ~ICANON; }
	else { tc.c_lflag |= ICANON; }

	tcsetattr(0, TCSANOW, &tc);
}

void global__SetAsyncIO(int enable)
{
	int fdFlags;

	fcntl(0, F_SETOWN, getpid());
	fdFlags = fcntl(0, F_GETFL);
	if (enable <= 0)
	{
		fcntl(0, F_SETFL, (fdFlags & ~O_ASYNC));
	}
	else
	{
		fcntl(0, F_SETFL, (fdFlags | O_ASYNC));
	}
}

void global__GetScreenSize(int* width, int* height)
{
	static int isInitialized = 0;
	struct winsize wbuf;

	if (isInitialized == 0)
	{
		if (ioctl(0, TIOCGWINSZ, &wbuf) == -1)
		{
			perror("global__GetScreenSize");
			exit(1);
		}
		isInitialized = 1;
	}

	*width = (int)wbuf.ws_col;
	*height = (int)wbuf.ws_row;
}

int system__ExecuteProgram(const char* iProgramPath, char* const iProgramArgs[], int isChild)
{
	if (isChild == M_NOTCHILD)
	{
		execvp(iProgramPath, iProgramArgs);
		return 0;
	}
	else if (isChild == M_CHILD)
	{
		int childPid = systm.ForkChild();
		if (childPid == -1)
		{
			perror("Failed to create child");
			return -1;
		}
		if (childPid == 0)
		{
			execvp(iProgramPath, iProgramArgs);
			// Rewritten child process never reach this code line.
			exit(1);
		}
		return childPid;
	}
	else
	{
		perror("isChild argument is not neither M_CHILD nor M_NOTCHILD");
		return -1;
	}
}

void system__GetWaitStatus(const int iWstatus, int* iReturnedValue, int* iDumpFlag, int* iSignalNumber)
{
	*iReturnedValue = (iWstatus & 0xFF00) >> 8;
	*iDumpFlag = (iWstatus & 0x80) >> 7;
	*iSignalNumber = (iWstatus & 0x7F);
}

int system__ForkChild()
{
	return fork();
}

int system__ForkChildExt(
		void (*iChildCb)(void* iChildParam), 
		void* iChildArg,
		void (*iParentCb)(void* iParentParam),
		void* iParentArg)
{
	int pidChild = systm.ForkChild();
	if (pidChild < 0) { return -1; }

	if (pidChild == 0)
	{
		iChildCb(iChildArg);
	}
	else
	{
		iParentCb(iParentArg);
	}
	return 0;
}

void assert__NeverReachThisLine(const char* iAdditionalMsg)
{
	if (iAdditionalMsg == NULL)
	{
		fprintf(stderr, 
				"This code must not be reached by any process of this program.\n"); 
	}
	else
	{
		fprintf(stderr, 
				"This code must not be reached by any process of this program. %s\n", 
				iAdditionalMsg); 
	}
	exit(1);
}

void assert__Fatal(char* iS1, char* iS2, int n)
{
  fprintf(stderr, "Error : %s, %s\n", iS1, iS2);
  exit(n);
}

void assert__Assert(int iResult, const char* iExpression, const char* iMsg)
{
	if (iResult == 0)
	{
		fprintf(stderr, "Failed assertion : %s, %s.\n", iExpression, iMsg);
		exit(1);
	}
}

void assert__MustSuccess(int iResult, const char* iMsg)
{
	if (iResult == -1)
	{
		fprintf(stderr, "This code must be succeeded but failed. %s\n", iMsg); 
		exit(1);
	}
}



