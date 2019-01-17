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

struct ArgSet* sMailBox = NULL;
pthread_mutex_t sMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  sCv	= PTHREAD_COND_INITIALIZER;

int main(int argc, char** argv)
{
	pthread_t t1, t2;
	void* CountWords(void*);

	struct ArgSet args1 = { argv[1], 0 };
	struct ArgSet args2 = { argv[2], 0 };
	
	pthread_mutex_lock(&sMutex);
	pthread_create(&t1, NULL, CountWords, &args1);
	pthread_create(&t2, NULL, CountWords, &args2);

	int reportsIn = 0;
	int totalWords = 0;
	while (reportsIn < 2)
	{
		printf("MAIN: waiting for flag to go up\n");
		pthread_cond_wait(&sCv, &sMutex);
		printf("MAIN: Wow! flag was raised, I have the lock\n");
		printf("%7d: %s\n", sMailBox->mCount, sMailBox->mFileName);

		totalWords += sMailBox->mCount;
		if (sMailBox == &args1) { pthread_join(t1, NULL); }
		if (sMailBox == &args2) { pthread_join(t2, NULL); }

		sMailBox = NULL;
		pthread_cond_signal(&sCv);
		reportsIn++;
	}

	printf("%20s : %5d\n", args1.mFileName, args1.mCount);
	printf("%20s : %5d\n", args2.mFileName, args2.mCount);
	printf("Total : %d\n", totalWords);
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

	printf("COUNT: waiting to get lock\n");
	pthread_mutex_lock(&sMutex);
	printf("COUNT: have lock, storing data\n");

	if (sMailBox != NULL) { pthread_cond_wait(&sCv, &sMutex); }
	sMailBox = iResult;

	printf("COUNT: raising flag\n");
	pthread_cond_signal(&sCv);
	printf("COUNT: unlocking box\n");
	pthread_mutex_unlock(&sMutex);
	sleep(0);

	return NULL;
}
