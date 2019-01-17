#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int counter = 0;

int main()
{
	pthread_t t1;
	void* PrintCount(void*);
	
	pthread_create(&t1, NULL, PrintCount, NULL);
	for (int i = 0; i < 10; ++i)
	{
		counter++;
		sleep(0); // Must specified.
	}

	pthread_join(t1, NULL);
	return 0;
}

void* PrintCount(void* _)
{
	for (int i = 0; i < 10; ++i)
	{
		printf("Count = %d\n", counter);
		sleep(0);
	}
	return NULL;
}
