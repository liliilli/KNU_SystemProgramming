#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int main()
{
	pthread_t t1, t2;
	void* printMsg(void*);

	pthread_create(&t1, NULL, printMsg, (void*)"Hello");
	pthread_create(&t2, NULL, printMsg, (void*)"World!");

	int* r1, * r2;

	pthread_join(t1, (void**)&r1);
	pthread_join(t2, (void**)&r2);

	printf("%d\n", (*r1) + (*r2));
	free(r1);
	free(r2);
	return 0;
}

void* printMsg(void* iArg)
{
	const char* cp = (const char*)iArg;
	for (int i = 0; i < 10; ++i)
	{
		printf("%s", cp);
		// `fflush` is Thread-safe. 
		// If fflush is not setup, does not guarantee output order.
		fflush(stdout); 
		sleep(0);
	}

	int* ret = (int*)malloc(sizeof(int));
	*ret = 10;
	return ret;
}
