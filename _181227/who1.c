#include <stdio.h>	/* ANSI C Header file */
#include <utmp.h>		/* User login information on now */
#include <fcntl.h>  /* Manipulate file decriptor */
#include <unistd.h> /* POSIX standard header file */
#include <stdlib.h> /* ANSI C library file */

#define SHOWHOST
#define FAILED_TO_READ -1

typedef struct utmp Utmp;
typedef Utmp*				TPtrUtmp;

void ShowInfo(TPtrUtmp);

int main(int argc, char** argv)
{
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
		   If Successful, just return readen bytes or FAILED_TO_READ. */
		ShowInfo(&currentRecord);
	}

	close(utmpFd);
	return 0;
}

char* gStrFmtNameLine = "%-8.8s ";

void ShowInfo(TPtrUtmp ptrUtmpBuffer)
{
	printf(gStrFmtNameLine, ptrUtmpBuffer->ut_name);
	printf(gStrFmtNameLine, ptrUtmpBuffer->ut_line);
	printf("%10d ", ptrUtmpBuffer->ut_time);
#if defined(SHOWHOST)
	printf("(%s)", ptrUtmpBuffer->ut_host);
#endif
	puts("");
}
