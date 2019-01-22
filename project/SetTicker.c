#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>
#include <signal.h>

int SetTicker(int iMsec)
{
	struct itimerval newTimeset;
	const long nSec = iMsec / 1000;
	const long nUsecs = (iMsec % 1000) * 1000L;

	newTimeset.it_interval.tv_sec = nSec;
	newTimeset.it_interval.tv_usec = nUsecs;
	newTimeset.it_value.tv_sec = nSec;
	newTimeset.it_value.tv_usec = nUsecs;

	return setitimer(ITIMER_REAL, &newTimeset, NULL);
}
