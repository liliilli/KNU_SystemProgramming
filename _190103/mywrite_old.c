/*
 * @file : mywrite.c
 * @author : Jongmin Yun
 * @description : 
 * 
 * Usage : ./mywrite name dest_tty_name
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <regex.h>
#include <unistd.h>
#include <fcntl.h>
#include <utmp.h> /* Login records of who is currentlu using the system. */

#define BOOL int
#define FALSE 0
#define TRUE 1
#define SUCCESS TRUE
#define FAILURE FALSE

BOOL GetCompactTtyName(const char* iFullname, char* iBufferCompactName, unsigned iBufLen)
{
	static const char* regExprStr = "^\\/dev\\/([a-zA-Z0-9\\/]*)";
	regex_t regex;
	regmatch_t regexMatch[2];

	/* Compile regular expression */ 
	int reti = regcomp(&regex, regExprStr, 0);
	if (reti)
	{
		fprintf(stderr, "Could not compile regex, %s\n", regExprStr);
		exit(1);
	}	

	/* Execute regular expression */
	reti = regexec(&regex, iFullname, 2, regexMatch, 0);
	if (!reti)
	{
#ifndef NDEBUG
		printf("Found : %c %c\n", iFullname + regexMatch[1].rm_so, iFullname = regexMatch[1].rm_eo);
#endif
	}
	
	return FAILURE;
}

BOOL CheckCurrentUsername(const char* iUserName)
{
	int fdUtmp;
	struct utmp currentRecord;
	int reclen = sizeof(struct utmp);
	BOOL isFound = FALSE;
	const char* currentFullTtyName = ttyname(0);
	char compactTtyName[256] = {'\0', }; 

	if (currentFullTtyName == NULL)
	{
		fprintf(stderr, "Failed to get current tty name.\n");
		exit(1);
	}
#ifndef NDEBUG
	printf("Current Full tty name : %s\n", currentFullTtyName);
#endif
	if (GetCompactTtyName(currentFullTtyName, compactTtyName, 256) == FAILURE)
	{
		fprintf(stderr, "Full tty name must be started with `/dev`");
		exit(1);
	}

	if ((fdUtmp = open(UTMP_FILE, O_RDONLY)) == -1)
	{
		perror(UTMP_FILE);
		exit(1);
	}

	while (read(fdUtmp, &currentRecord, reclen) == reclen)
	{
#ifndef NDEBUG
		printf("%s %s\n", currentRecord.ut_user, currentRecord.ut_line);
#endif
		
	}

	close(fdUtmp);
	return SUCCESS;
}

int main(int argc, char** argv)
{
	int fd;
	char buffer[512];
	struct utmp currentRecord;

	BOOL flag = CheckCurrentUsername(argv[1]);
	if (flag == FALSE)
	{
		fprintf(stderr, "Failed to find proper user_line");
		exit(1);
	}

	/* Check args */
	if (argc != 3) 
	{
		fprintf(stderr, "Usage : Write0 user_name ttyname\n");
		exit(1);
	}	

	/* Open device */
	fd = open(argv[1], O_WRONLY);
	if (fd == -1)
	{
		perror(argv[1]); 
		exit(1);
	}

	/* Loop until EOF on input */
	while (fgets(buffer, 512, stdin) != NULL)
	{
		if (write(fd, buffer, strlen(buffer)) == -1) { break; }
	}

	close(fd);
}
