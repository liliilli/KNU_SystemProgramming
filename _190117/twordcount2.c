#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "HelperGlobal.h"

pthread_mutex_t sCounterLock = PTHREAD_MUTEX_INITIALIZER;
int sTotalWords = 0;

/// @brief This function is thread-safe.
int increaseWordCount()
{
	pthread_mutex_lock(&sCounterLock);
	sTotalWords++;
	pthread_mutex_unlock(&sCounterLock);
}

int main(int argc, char** argv)
{
	pthread_t t1, t2;
	void* CountWords(void*);
	
	pthread_create(&t1, NULL, CountWords, argv[1]);
	pthread_create(&t2, NULL, CountWords, argv[2]);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("Total : %d\n", sTotalWords);
	return 0;
}

void* CountWords(void* iFileName)
{
	char* filename = (char*)iFileName;
	FILE* fp = fopen(filename, "r");
	assert.MustSuccess(fp != NULL, "Fatal. Failed to read file.");

	char c = '\0';
	char prevc = '\0';
	while ((c = getc(fp)) != EOF)
	{
		if (isalnum(c) == 0 && isalnum(prevc) != 0)
		{ // Critical section.
			increaseWordCount();
		}
		prevc = c;
	}

	fclose(fp);
}
