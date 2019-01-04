#include <stdio.h>

#include <termios.h>
#include <unistd.h>

#define SAVE_CURRENT_MODE 0
#define RESTORE_MODE 1

const char* gQuestionString = "Do you want another transaction";

int GetResponse(const char*);
void SetCharMode();
void TtyMode(int);

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	TtyMode(SAVE_CURRENT_MODE);
	SetCharMode();
	int response = GetResponse(gQuestionString);
	TtyMode(RESTORE_MODE);

	return response;
}

int GetResponse(const char* iQuestion)
{
	printf("%s (y/n)?", iQuestion);
	char input;
	while (1)
	{
		switch (input = getchar())
		{
			case 'y': case 'Y': return 0;
			case 'n': case 'N': return 1;
			case EOF: return 1;
			default:
			{
				printf("\nCould not understand %c ", input);
				printf("Plase type `y``Y``n` or `N`.\n");
			} break;
		}
	}
}

void SetCharMode()
{
	struct termios ttyState;
	tcgetattr(0, &ttyState);
	ttyState.c_lflag &= ~(ICANON);
	ttyState.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &ttyState);
}

void TtyMode(int how)
{
	static struct termios originalMode;
	if (how == SAVE_CURRENT_MODE) { tcgetattr(0, &originalMode); }
	else { tcsetattr(0, TCSANOW, &originalMode); }
}

