///
/// @file : timeservc.c
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-15 
/// @description : 
///

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "HelperGlobal.h"

#define M_HOSTLEN 256
#define M_PORTNUM 13000

int main(int argc, char** argv)
{
	struct sockaddr_in sAddr;
	struct hostent*    sHp;

  char    hostName[M_HOSTLEN];
  int     socketId;
  int     socketFd;
  FILE*   socketFp = NULL;
  time_t  theTime;

	// Step 1 : ask kernel for socket.
	// Get a socket.
	assert.MustSuccess((socketId = socket(PF_INET, SOCK_STREAM, 0)), "Failed to get a socket");

	// Step 2 : bind address to socke.t Address is host, post.
	// Clear out struct.
	bzero((void*)&sAddr, sizeof(sAddr));
	// Get a host name of me.
	gethostname(hostName, M_HOSTLEN);
	sHp = gethostbyname(hostName);

	bcopy((void*)sHp->h_addr, (void*)&sAddr.sin_addr, sHp->h_length);
	sAddr.sin_port = htons(M_PORTNUM);
	sAddr.sin_family = AF_INET;

	if (bind(socketId, (struct sockaddr*)&sAddr, sizeof(sAddr)) != 0)
	{
		assert.Fatal("Bind", "Bind", 1);
	}

	// Step 3 : allow incoming calls with Qsize = 1 on socket.
	if (listen(socketId, 1) != 0) { assert.Fatal("Listen", "Listen", 1); }

	// Main loop
	while (1)
	{
		// Wait for call.
		socketFd = accept(socketId, NULL, NULL);
		printf("Wow! got a call!\n");
		M_ASSERT(socketFd != -1, "Accept");

		socketFp = fdopen(socketFd, "w");
		M_ASSERT(socketFp != NULL, "Fdopen");

		theTime = time(NULL);
		fprintf(socketFp, "The time here is...");
		fprintf(socketFp, "%s", ctime(&theTime));
		fclose(socketFp);
	}

	return 0;
}

