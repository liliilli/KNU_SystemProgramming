#include <stdio.h>

const char* gQuestionString = "Do you want another transaction";

int getResponse(const char*);

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	int response = getResponse(gQuestionString);
	return response;

	return 0;
}

int getResponse(const char* iQuestion)
{
	printf("%s (y/x)?", iQuestion);
	while (1)
	{
		switch (getchar())
		{
			case 'y': case 'Y':
				return 0;
			case 'n': case 'N':
				return 1;
		}
	}
}
