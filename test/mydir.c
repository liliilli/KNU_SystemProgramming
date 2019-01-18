//
// @file : mydir.c
// @author : 2012104208 Jongmin Yun
// @date : 2019-01-18 10:03
// @description : 
//

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h> // Pipe communication
#include <signal.h> // 
#include <fcntl.h>  // File controlling 
#include <dirent.h> // Directory searching 

const char* sLogFilePath		= "logfile.log";
const char* sDirectoryPath;
const char* sCommStringSort = "sort";
const char* sCommStringFind = "find";
const char* sCommStringExit = "quit";

// @enum ECommandStatus
// @brief Command value status.
enum ECommandStatus
{
	Comm_Sort,	// Sort command
	Comm_Find,	// Find command
	Comm_Quit,	// Quit command
	__Comm_Error// MUST NOT BE THIS VALUE.
};

//! - - - - - - - - - - - 
//! INTERFACE FUNCTIONS
//! - - - - - - - - - - - 

// @brief Just print instruction.
void PrintInstruction();
// @brief Let user input command string, and get proper command value as return value.
enum ECommandStatus GetCommand();

//! - - - - - - - - - - - 
//! SETUP FUNCTIONS
//! - - - - - - - - - - - 

void InitializeProgram();
void __InitializeAlarm();

void TerminateProgram();
void __TerminateAlarm();

//! - - - - - - - - - - - 
//! ALARM FUNCTION
//! - - - - - - - - - - - 

void CallbackAlarm(int _);

//! - - - - - - - - - - - 
//! LS SORT FUNCTION
//! - - - - - - - - - - - 

void ProcessCommandSort();

//! - - - - - - - - - - - 
//! FIND FUNCTION
//! - - - - - - - - - - - 

void ProcessCommandFind()
{
	static char filename[BUFSIZ];

	printf("file name to search : ");
	fflush(stdin);
	char* ptrResult = fgets(filename, BUFSIZ, stdin); fflush(stdin);

	if (ptrResult == NULL)
	{ // If error has occurred, let user write command again.
		fprintf(stderr, "Failed to get a file name string. Please try again.\n");
		exit(1);
	}
	else
	{ // Replace \n with \0.
		const int len = strlen(filename);
		filename[len - 1] = '\0';
	}

	// Make pipe.
	int findPipe[2];
	if (pipe(findPipe) == -1)
	{
		fprintf(stderr, "Failed to create pipe. Please try again\n");
		return;
	}

	// Make child process for IPC.
	int childProcess = fork();
	if (childProcess == -1)
	{
		fprintf(stderr, "Failed to fork child process. Please try again.\n");
		return;
	}

	if (childProcess == 0)
	{ // If child process, execute find with directory and filename. `find dir filename`.
		close(findPipe[0]);
		if (dup2(findPipe[1], 1) == -1)
		{
			fprintf(stderr, "Fatal. Failed to connect pipe to stdout.\n");
			exit(1);
		}
		close(findPipe[1]);
		
		execlp("find", sDirectoryPath, "-name", filename, NULL);
		exit(1);
	}
	else
	{ // If parent process, get result from child process,
		// check there is no result or any result.
		int oldStdin = dup(0);
		close(findPipe[1]);
		if (dup2(findPipe[0], 0) == -1)
		{
			fprintf(stderr, "Fatal. Failed to connect pipe to stdin.\n");
			exit(1);
		}
		close(findPipe[0]);
		
		int count = 0;
		char buffer[BUFSIZ];
		while (read(0, buffer, BUFSIZ) > 0) { ++count; }
		fflush(stdin);

		if (count > 0) { printf("Find result : Succeeded in searching, `%s`.\n", filename); }
		else           { printf("Find result : Failed to search file, `%s`.\n", filename); }
		dup2(oldStdin, 0);
	}
}

int main(int argc, char** argv)
{
	// (1) Expected argc count is 2.
	if (argc != 2)
	{
		fprintf(stderr, "Failed to execute program, use like `./mydir directory_path`");
		exit(1);
	}

	// (1) Check there is a directory.
	sDirectoryPath = argv[1];
	DIR* dirInfo = opendir(sDirectoryPath);

	InitializeProgram();
	PrintInstruction();

	// (1) Get command.
	int bIsTerminated = 0;
	while (bIsTerminated == 0)
	{
		enum ECommandStatus commandStatus = GetCommand();

		switch (commandStatus)
		{
		case Comm_Sort: { ProcessCommandSort(); } break;
		case Comm_Find: { ProcessCommandFind(); } break;
		// We need to exit and release all heap instance and restore all properties.
		case Comm_Quit: { bIsTerminated = 1; } break;
		default: break;
		}
	}

	// (2) (10ptr) Terminate program when inputted `quit`.
	TerminateProgram();
	closedir(dirInfo);
}

//! - - - - - - - - - - - 
//! INTERFACE FUNCTIONS
//! - - - - - - - - - - - 

void PrintInstruction()
{
	printf(
			"===== myfinal (2012104208 / Jongmin Yun) =====\n"
			"Directory monitoring program : %s\n"
			"===== commands =====\n"
			"ls sorting : %s\n"
			"find file : %s\n"
			"program exit : %s\n", 
			sDirectoryPath, sCommStringSort, sCommStringFind, sCommStringExit);
}

enum ECommandStatus GetCommand()
{
	static char command[BUFSIZ];

	while (1)
	{
		printf("Command >> ");

		char* ptrResult = fgets(command, BUFSIZ, stdin); fflush(stdin);
		if (ptrResult == NULL)
		{ // If error has occurred, let user write command again.
			fprintf(stderr, "Failed to get a command string. Please try again.\n");
			continue;
		}

		// Replace \n with \0.
		const int len = strlen(command);
		command[len - 1] = '\0';

		// Compare and get command value.
		if (strcmp(command, sCommStringSort) == 0) { return Comm_Sort; }
		if (strcmp(command, sCommStringFind) == 0) { return Comm_Find; }
		if (strcmp(command, sCommStringExit) == 0) { return Comm_Quit; }
	}

	return __Comm_Error;
}

//! - - - - - - - - - - - 
//! SETUP FUNCTIONS
//! - - - - - - - - - - - 

void InitializeProgram()
{
	__InitializeAlarm();
}

void __InitializeAlarm()
{
	void CallbackAlarm(int);

	// Get real timer for logging and comparing.
	// log file must be activated with `auto-append` mode for synchronization.
	struct itimerval alarmTimer;
	signal(SIGALRM, CallbackAlarm); 
	if (getitimer(ITIMER_REAL, &alarmTimer) == -1)
	{
		fprintf(stderr, "Unexpected error occurred. Failed to get alarm information.\n");
		exit(1);
	}

	alarmTimer.it_value.tv_sec = 1;
	alarmTimer.it_interval.tv_sec = 1;

	if (setitimer(ITIMER_REAL, &alarmTimer, NULL) == -1)
	{
		fprintf(stderr, "Unexpected error occurred. Failed to set alarm information.\n");
		exit(1);
	}
}

void TerminateProgram()
{
	__TerminateAlarm();
}

void __TerminateAlarm()
{
	struct itimerval alarmTimer;
	signal(SIGALRM, SIG_IGN); 
	if (getitimer(ITIMER_REAL, &alarmTimer) == -1)
	{
		fprintf(stderr, "Unexpected error occurred. Failed to get alarm information.\n");
		exit(1);
	}

	alarmTimer.it_value.tv_sec = 0;
	alarmTimer.it_interval.tv_sec = 0;

	if (setitimer(ITIMER_REAL, &alarmTimer, NULL) == -1)
	{
		fprintf(stderr, "Unexpected error occurred. Failed to set alarm information.\n");
		exit(1);
	}
}

//! - - - - - - - - - - - 
//! ALARM FUNCTION
//! - - - - - - - - - - - 

void CallbackAlarm(int _)
{
	(void)_;
}

void ProcessCommandSort()
{
	struct dirent** entryList = NULL;
	int count = scandir(sDirectoryPath, &entryList, NULL, alphasort);
	if (count == -1)
	{
		fprintf(stderr, "Failed to execute sort. Could not get entries of given directory path.\n");
		return;
	}
	else
	{
		for (int i = 0; i < count; ++i) 
		{ 
			printf("%s\n", entryList[i]->d_name); 
			free(entryList[i]);
		}
		free(entryList);
	}
}


