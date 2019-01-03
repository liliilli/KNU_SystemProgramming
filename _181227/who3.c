/*
 * @file : who3.c
 * @author : Jongmin Yun
 * @datetime : 2018-12-27 23:33 Updated.
 * @description : The program immitiating `who` program of -nixes.
 */

#include <stdio.h>  /* ANSI C Header file */
#include <stdlib.h> /* ANSI C library file */
#include <time.h>

#include <fcntl.h>  /* Manipulate file decriptor */
#include <unistd.h> /* POSIX standard header file */
#include <utmp.h>   /* User login information on now */

#include "utmplib.h"/* Header file to call utmplib functions. */ 

#define SHOWHOST

enum EErrorCode
{ /* Use this at everywhere which general error code is -1. */
  E_Failed = -1
};

/* @brief Print information of utmp. */ 
void ShowInfo(struct utmp*);
/* @brief Print time string. */
void ShowTime(time_t iSec);

int main(int argc, char** argv)
{
  struct utmp* ptrUtmpBuffer = NULL;

  /* Try open utmp file as default utmp file path. */
  if (utmp_open(UTMP_FILE) == E_Failed)
  { /* If program filed to read /var/run/utmp, or specified default utmp file,
     * just print UTMP_FILE (default utmp path) and abort. */
    perror(UTMP_FILE);
    exit(1);
  }

  /* Try get next user log-in information. 
   * If succeeded, show information of retrieved user. */
  while ((ptrUtmpBuffer = utmp_next()) != NULL) { ShowInfo(ptrUtmpBuffer); }

  /* utmp buffer must be released manually. */
  utmp_close();
  return 0;
}

void ShowInfo(struct utmp* ptrUtmpBuffer)
{
  /* If retrieved utmp information type is not user, do nothing. */
  if (ptrUtmpBuffer->ut_type != USER_PROCESS) { return; }

  printf("%-8.8s ", ptrUtmpBuffer->ut_name);
  printf("%-8.8s ", ptrUtmpBuffer->ut_line);
  /* Unix time_t (implementation dependent) can be created from int32_t. 
   * utmp::ut_time (timestamp second) is int32_t. */
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
