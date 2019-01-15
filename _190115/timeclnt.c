///
/// @file : timeclnt.c
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

	char message[BUFSIZ];

	// Step 1 : ask kernel for socket.
	// Get a socket.
	assert.MustSuccess((socketId = socket(AF_INET, SOCK_STREAM, 0)), "Failed to get a socket");

	// Step 2 : connect to server 
	// Need to build address (host, port) of server first.
	bzero((void*)&sAddr, sizeof(sAddr));

	// Get a host name of me.
	sHp = gethostbyname(argv[1]);
	M_ASSERT(sHp != NULL, "Failed to connect to server");

	bcopy((void*)sHp->h_addr, (void*)&sAddr.sin_addr, sHp->h_length);
	sAddr.sin_port = htons(atoi(argv[2]));
	sAddr.sin_family = AF_INET;

	if (connect(socketId, (struct sockaddr*)&sAddr, sizeof(sAddr)) != 0)
	{
		assert.Fatal("Connect", "Connect", 1);
	}

	// Step 3 : allow incoming calls with Qsize = 1 on socket.
	int messageLen = read(socketId, message, BUFSIZ);
	M_ASSERT(messageLen != -1, "Read error");

	if (write(1, message, messageLen) != messageLen)
	{
		assert.Fatal("Write Error failed", "Write Error failed", 1);
	}

	close(socketId);
	return 0;
}

