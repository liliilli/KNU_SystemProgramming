#include <stdio.h>	/* ANSI C Header file */
#include <utmp.h>		/* User login information on now */
#include <fcntl.h>  /* Manipulate file decriptor */
#include <unistd.h> /* POSIX standard header file */
#include <stdlib.h> /* ANSI C library file */
#include <time.h>
#include "utmplib.h"

#define SHOWHOST
#define FAILED_TO_READ -1

typedef struct utmp Utmp;
typedef Utmp*				TPtrUtmp;

void ShowInfo(TPtrUtmp);
void ShowTime(time_t iSec);

int main(int argc, char** argv)
{
	struct utmp* ptr_utmpbuff;

	if (utmp_open(UTMP_FILE) == -1)
	{ /* If program filed to read /var/run/utmp... */
		perror(UTMP_FILE);
		return 0;
	}

	while ((ptr_utmpbuff = utmp_next()) != NULL)
	{
		ShowInfo(ptr_utmpbuff);
	}

	utmp_close();
	return 0;
}

char* gStrFmtNameLine = "%-8.8s ";

void ShowInfo(TPtrUtmp ptrUtmpBuffer)
{
	if (ptrUtmpBuffer->ut_type != USER_PROCESS) { return; }

	printf(gStrFmtNameLine, ptrUtmpBuffer->ut_name);
	printf(gStrFmtNameLine, ptrUtmpBuffer->ut_line);
	ShowTime(ptrUtmpBuffer->ut_time);
#if defined(SHOWHOST)
	if (ptrUtmpBuffer->ut_host[0] != '\0')
	{
		printf("(%s)", ptrUtmpBuffer->ut_host);
	}
#endif
	puts("");
}

void ShowTime(time_t iSec)
{
	char* timePointString = ctime(&iSec);
	printf("%12.12s ", timePointString + 4);
}
