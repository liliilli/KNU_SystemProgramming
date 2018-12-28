/*
 * @file : myls.c
 * @author : 2012104208_Jongmin Yun
 * @datetime : 2018-12-28 12:01
 * @decription : Immitation program of `ls`.
 * To use this, input directory path next to execution command of program.
 * 
 * Example)
 * Command : 
 * >> ./myls .
 * Result : 
 * .: 
	drwxr-xr-x    3 swedu    swedu        4096 Dec 28 11:37 .
	drwxr-xr-x    7 swedu    root         4096 Dec 28 08:53 ..
	-rw-r--r--    1 swedu    swedu        3680 Dec 28 11:36 myls.c
	-rwxr-xr-x    1 swedu    swedu       13424 Dec 28 11:34 myls
	-rwxr-xr-x    1 swedu    swedu       13288 Dec 28 11:34 ls2
	-rw-r--r--    1 swedu    swedu           0 Dec 28 11:37 file
	-rw-r--r--    1 swedu    swedu        5136 Dec 28 11:34 ls2.o
	-rw-r--r--    1 swedu    swedu         843 Dec 28 09:35 ls1.c
	// ...
	-rwxr-xr-x    1 swedu    swedu        8576 Dec 28 09:35 ls1
 *
 * @fixed
 * This program fixed the problem that not finding files when searching other directories,
 * when using `ls2` program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * @brief Process files and get attribute, print them.
 */
void ProcessLs(const char* iDirName);

/*
 * @brief Try Get staus information and store information stat ptr address into iPPtrStatus.
 * If successed, return 0 otherwise -1.
 */
int TryGetStatusInfo(const char* iDirName, const char* iPath, struct stat** iPPtrStatus);

/*
 * @brief Show status information of given iPath.
 */
void ShowStatusInfo(const char* iPath, struct stat* iPtrStatus);

/*
 * @brief Convert file's mode to string like a `drwxrwxrwx`.
 */
int ConvertModeToLetters(mode_t iMode, char* iString);

/*
 * @brief Get user's name of specified file uid.
 * If not found, just return "" (empty string).
 */
const char* GetUserString(uid_t iUid);

/*
 * @brief Get group's name of specified file gid.
 * If not found, just return "" (empty string).
 */
const char* GetGroupString(gid_t iGid);

int main(int argc, char** argv)
{
	int i;

	/* If no additional argument, just search `./` directory. */
	if (argc == 1) { ProcessLs("."); }
	else
	{ /* If given additional arguments, search file of each directory path */
		for (i = 1; i < argc; ++i)
		{
			printf("[[Search `%s` directory]] \n", argv[i]);
			ProcessLs(argv[i]);
		}
	}

	return 0;
}

void ProcessLs(const char* iDirName)
{
	DIR* ptrDir = NULL;
	struct dirent* ptrDirent = NULL;
	struct stat infoBuf;
	struct stat* ptrInfoBuf = &infoBuf;

	if ((ptrDir = opendir(iDirName)) == NULL)
	{
		fprintf(stderr, "ls1 : Failed to open directory %s\n", iDirName);
		return;
	}

	while ((ptrDirent = readdir(ptrDir)) != NULL)
	{	/* If failed to status information from file, just pass it. */
		if (TryGetStatusInfo(iDirName, ptrDirent->d_name, &ptrInfoBuf) == -1)
		{
			fprintf(stderr, "ls2 : Failed to retrieve information of file %s\n", ptrDirent->d_name);
			continue;
		}
		
		ShowStatusInfo(ptrDirent->d_name, &infoBuf);
	}
	
	if (closedir(ptrDir) == -1)
	{ 
		fprintf(stderr, "ls1 : Failed to close directory sturcture instance %s\n", iDirName);
	}
}

int TryGetStatusInfo(const char* iDirName, const char* iPath, struct stat** iPPtrStatus)
{
	/* Make semi-full path of file to get information of file */
	const unsigned lenFullPath = strlen(iDirName) + strlen(iPath) + 1 + 1;
	char* fullpath = (char*)calloc(lenFullPath, sizeof(char));
	strcat(fullpath, iDirName);
	strcat(fullpath, "/");
	strcat(fullpath, iPath);

	if (stat(fullpath, *iPPtrStatus) == -1) 
	{ /* If failed to get status of file, return with error flag -1. */
		perror(iPath);
		free(fullpath);
		return -1; 
	}

	free(fullpath);
	return 0;
}

void ShowStatusInfo(const char* iPath, struct stat* iPtrStatus)
{
	char modeString[11] = "----------";
	if (ConvertModeToLetters(iPtrStatus->st_mode, modeString) == -1)
	{ /* When failed to get mode string, just let it be empty mode string. */
		strcpy(modeString, "          ");
	}

	/* Print string */
	printf("%10s ", modeString);
	printf("%4lu ", iPtrStatus->st_nlink);
	printf("%-8s ", GetUserString(iPtrStatus->st_uid));
	printf("%-8s ", GetGroupString(iPtrStatus->st_gid));
	printf("%8ld ", (long int)(iPtrStatus->st_size));
	printf("%.12s ", ctime(&iPtrStatus->st_mtime) + 4);
	printf("%s\n", iPath);
}

int ConvertModeToLetters(mode_t iMode, char* iString)
{
	if (strlen(iString) != 10)
	{ /* given string to update with mode character must be 10 characters without NULL */
		fprintf(stderr, "Failed to retrieve mode characters from mode value.");
		return -1;
	}

	if (S_ISDIR(iMode)) { iString[0] = 'd'; }
	if (S_ISCHR(iMode)) { iString[0] = 'c'; }
	if (S_ISBLK(iMode)) { iString[0] = 'b'; }

	if (iMode & S_IRUSR) iString[1] = 'r';
	if (iMode & S_IWUSR) iString[2] = 'w';
	if (iMode & S_IXUSR) iString[3] = 'x';

	if (iMode & S_IRGRP) iString[4] = 'r';
	if (iMode & S_IWGRP) iString[5] = 'w';
	if (iMode & S_IXGRP) iString[6] = 'x';

	if (iMode & S_IROTH) iString[7] = 'r';
	if (iMode & S_IWOTH) iString[8] = 'w';
	if (iMode & S_IXOTH) iString[9] = 'x';
	return 0;
}

const char* GetUserString(uid_t iUid)
{
	struct passwd* ptrUser = getpwuid(iUid);

	if (ptrUser == NULL) { return ""; }
	else { return ptrUser->pw_name; }
}

const char* GetGroupString(gid_t iGid)
{
	struct group* ptrGroup = getgrgid(iGid);

	if (ptrGroup == NULL) { return ""; }
	else { return getgrgid(iGid)->gr_name; }
}
