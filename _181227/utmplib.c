#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <utmp.h>

#define NRECS		16
#define UTSIZE	(sizeof(struct utmp))
#define NULLUT	((struct utmp*)NULL)

typedef int					TFileDesc;

TFileDesc			utmp_open(const char* iFileName);
struct utmp*	utmp_next();
int						utmp_reload();
void					utmp_close();

static char	utmpbuf[NRECS * UTSIZE];
static int num_recs;
static int cur_rec;
static TFileDesc fd_utmp = -1;

TFileDesc
utmp_open(const char* iFileName)
{
	fd_utmp = open(iFileName, O_RDONLY);
	cur_rec = num_recs = 0;
	return fd_utmp;
}

struct utmp*
utmp_next()
{
	struct utmp* ptr_rec;
	if (fd_utmp == -1) { return NULLUT; }
	if (cur_rec == num_recs && utmp_reload() == 0) { return NULLUT; }

	ptr_rec = (struct utmp*) &utmpbuf[cur_rec * UTSIZE];
	cur_rec += 1;
	return ptr_rec;
}

int utmp_reload()
{
	int amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
	num_recs = amt_read / UTSIZE;
	cur_rec  = 0;
	return num_recs;
}

void utmp_close()
{
	if (fd_utmp != -1)
	{
		close(fd_utmp);
	}
}

