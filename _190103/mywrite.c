/*
 * @file : mywrite.c
 * @author : 2012104208 Jongmin Yun
 * @date : 2018-01-03 11:58 Created
 *         2018-01-03 23:39 Update comment
 * @description : Chatting program with other client side.
 * 
 * Usage : ./mywrite name dest_tty_name
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>
#include <utmp.h> /* Login records of who is currentlu using the system. */

#define BOOL int
#define FALSE 0
#define TRUE 1
#define SUCCESS TRUE
#define FAILURE FALSE

BOOL CheckCurrentUsername(const char* iUserName)
{
	const uid_t id = geteuid();
	struct passwd* userPw = getpwuid(id);

	if (strcmp(iUserName, userPw->pw_name) != 0) { return FAILURE; }
	return SUCCESS;
}

int main(int argc, char** argv)
{
	int fd;
	char stringBuffer[512];
	char headerFormat[512];
	time_t timeStmp;

	/* Check args */
	if (argc != 3) 
	{
		fprintf(stderr, "Usage : Write0 user_name ttyname\n");
		exit(1);
	}	

	BOOL flag = CheckCurrentUsername(argv[1]);
	if (flag == FAILURE)
	{
		fprintf(stderr, "Failed to find proper user_line");
		exit(1);
	}

	/* Open device */
	fd = open(argv[2], O_WRONLY);
	if (fd == -1)
	{
		perror(argv[2]); 
		exit(1);
	}

	/* Loop until EOF on input */
	while (fgets(stringBuffer, 512, stdin) != NULL)
	{
		timeStmp = time(NULL);
		struct tm buf;
		buf = *localtime(&timeStmp);

		/* Make log header for each message */
		memset(headerFormat, '\0', 512);
		sprintf(headerFormat, "\nMessage from %s at %d : %d ...\n\n",
			argv[1],
			buf.tm_hour,
			buf.tm_min);

		/* Write header log to another /pts/N */
		if (write(fd, headerFormat, strlen(headerFormat)) == -1) { break; }
		/* Write content */
		if (write(fd, stringBuffer, strlen(stringBuffer)) == -1) { break; }
	}

	/* Write `EOF` when exist chatting with CTRL+D. */
	if (write(fd, "EOF", strlen("EOF")) == -1) { exit(1); }
	close(fd);
}
