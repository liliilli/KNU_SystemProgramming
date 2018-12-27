#include <stdio.h>	/* ANSI C Header file */
#include <stdlib.h> /* ANSI C library file */
#include <time.h>		/* Time Header file of ANSI C */

#include <utmp.h>		/* User login information on now */
#include <fcntl.h>  /* Manipulate file decriptor */
#include <unistd.h> /* POSIX standard header file */

#define SHOWHOST
#define FAILED_TO_READ -1

typedef struct utmp Utmp;
typedef Utmp*				TPtrUtmp;

void ShowTime(time_t);
void ShowInfo(TPtrUtmp);

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	Utmp currentRecord;
	int	 utmpFd;
	int	 reclen = sizeof(currentRecord);

	if ((utmpFd = open(UTMP_FILE, O_RDONLY)) == FAILED_TO_READ)
	{ /* If program filed to read /var/run/utmp... */
		perror(UTMP_FILE);
		return 0;
	}

	while (read(utmpFd, &currentRecord, reclen) == reclen)
	{ /* Read struct utmp with reclen (size of struct utmp) bytes.
		   If Successful, just return readen bytes or 0 when EOF.
			 otherwise, return FAILED_TO_READ. */
		ShowInfo(&currentRecord);
	}

	close(utmpFd);
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
