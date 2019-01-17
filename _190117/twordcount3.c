#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "HelperGlobal.h"

struct ArgSet
{
	char* mFileName;
	int   mCount;
};

int main(int argc, char** argv)
{
	pthread_t t1, t2;
	void* CountWords(void*);

	struct ArgSet args1 = { argv[1], 0 };
	struct ArgSet args2 = { argv[2], 0 };
	
	pthread_create(&t1, NULL, CountWords, &args1);
	pthread_create(&t2, NULL, CountWords, &args2);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("%20s : %5d\n", args1.mFileName, args1.mCount);
	printf("%20s : %5d\n", args2.mFileName, args2.mCount);
	printf("Total : %d\n", args1.mCount + args2.mCount);
	return 0;
}

void* CountWords(void* iInput)
{
	struct ArgSet* iResult = (struct ArgSet*)iInput;
	assert.MustSuccess(iResult->mFileName != NULL, "File name must be specified.");

	FILE* fp = fopen(iResult->mFileName, "r");
	assert.MustSuccess(fp != NULL, "Fatal. Failed to read file.");

	char c = '\0';
	char prevc = '\0';
	while ((c = getc(fp)) != EOF)
	{
		if (isspace(c) && isprint(prevc) != 0)
		{ // Critical section.
			iResult->mCount++;
		}
		prevc = c;
	}

	fclose(fp);
}
