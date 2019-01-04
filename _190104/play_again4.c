#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

#define SAVE_CURRENT_MODE 0
#define RESTORE_MODE 1

const char* gQuestionString = "Do you want another transaction";

int GetResponse(const char*);
void SetCharMode();
void SetNoDelayMode();
void TtyMode(int);
void CbSigCtrlCHandler(int signum);
#ifndef NDEBUG
//typedef struct __FlagInfo { int flValue };
//void __ShowFlagSet(const int iFlagValue, __FlagInfo* iFlagInfoList);
#endif

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	TtyMode(SAVE_CURRENT_MODE);
	SetCharMode();
	SetNoDelayMode();

	signal(SIGINT, CbSigCtrlCHandler);
	signal(SIGQUIT, SIG_IGN);

	int response = GetResponse(gQuestionString);
	TtyMode(RESTORE_MODE);

	return response;
}

void CbSigCtrlCHandler(int signum)
{
	(void)signum;
	TtyMode(RESTORE_MODE);
	exit(1);
}

int GetResponse(const char* iQuestion)
{
	int maxTries = 3;
	printf("%s (y/n)?", iQuestion);
	fflush(stdout);

	while (1)
	{
		sleep(2);
		int input;
		while ((input = getchar()) != EOF && strchr("yYnN", input) == NULL)
			;

		input = tolower(input);
		switch (input)
		{
			case 'y': return 0;
			case 'n': return 1;
			default:
			{
				if (maxTries == 0) { return 2; } else { maxTries--; }
			} break;
		}

		putchar(input);
	}
}

void SetCharMode()
{
	struct termios ttyState;
	tcgetattr(0, &ttyState);
	ttyState.c_lflag &= ~(ICANON);
	ttyState.c_lflag &= ~(ECHO);
	ttyState.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &ttyState);

#ifndef NDEBUG

#endif
}

void SetNoDelayMode()
{
	int ttyFlags = fcntl(0, F_GETFL);
	ttyFlags |= O_NDELAY;
	fcntl(0, F_SETFL, ttyFlags);
}

void TtyMode(int how)
{
	static struct termios originalMode;
	static int originalFlags;
	static int isPropertiesStored = 0;

	if (how == SAVE_CURRENT_MODE) 
	{ 
		tcgetattr(0, &originalMode); 
		originalFlags = fcntl(0, F_GETFL);
		isPropertiesStored = 1;
	}
	else if (isPropertiesStored == 1)
	{ 
		tcsetattr(0, TCSANOW, &originalMode); 
		originalFlags = fcntl(0, F_SETFL, originalFlags);
	}
}

