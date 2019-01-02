/*
 * @file myspwd.c
 * @author 2012104208_JongminYun
 * @datetime 2019-01-02 10:55
 * @description Fixed `spwd` program which did not display anything when called on `root` directory.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

/*
 * @brief Close directory stream safely.
 */ 
#define CLOSE_DIR_SAFELY(__MADIR__) \
	if (closedir(__MADIR__) == -1) \
	{ fprintf(stderr, "Failed to close directory stream\n"); exit(1); } \
	__MADIR__ = NULL

#define M_PARSE2(__MAPrefix__, __MASuffix__) __MAPrefix__##__MASuffix__
#define PRIVATE(__MAFunction__) M_PARSE2(__, __MAFunction__)

/*
 * @brief This macro will be used when given expression is not used in production code 
 * but use to avoid compile error when build a program.
 */
#define MACRO_NOTUSED_VARIABLE_BUT_USE_ANYWAY(__MAExpression__) \
	((void)(__MAExpression__))

/*
 * @brief Get inode number of given path file.
 */
ino_t GetInodeNumber(const char* iPathName)
{
	struct stat resultInfo;
	if (stat(iPathName, &resultInfo) == -1)
	{
		fprintf(stderr, "Failed to stat ");
		perror(iPathName);
		exit(1);
	}

	return resultInfo.st_ino;
}

/*
 * @brief Convert given inode number with name, 
 * converted name will be inserted in iNameBuffer.
 */
void ConvertInodeToName(const ino_t iExpectedInode, char* iNameBuffer, int iBufferLen)
{
	DIR* ptrDir = NULL;
	struct dirent* ptrDirent = NULL;
	if ((ptrDir = opendir(".")) == NULL) { perror("."); exit(1); }

	/* Search directory for a file specified iNode number. */
	while ((ptrDirent = readdir(ptrDir)) != NULL)
	{
		if (ptrDirent->d_ino == iExpectedInode)
		{
			strncpy(iNameBuffer, ptrDirent->d_name, iBufferLen);
			iNameBuffer[iBufferLen - 1] = '\0';
			CLOSE_DIR_SAFELY(ptrDir);
			return;
		}
#ifndef NDEBUG
		else { printf("%s\n", ptrDirent->d_name); }
#endif
	}

	fprintf(stderr, "Error looking for iNode number %lu\n", iExpectedInode);
	exit(1);
}

void PRIVATE(PrintFullPathTo)(const ino_t iInode, const int lv)
{
	/* Validation check */
	if (GetInodeNumber("..") == iInode) { printf("/"); return; }

	if (chdir("..") == -1) { fprintf(stderr, "Unexpected error occurred\n"); exit(1); }
	char itsName[512];
	/* Recurse until hit root directory. */
	ConvertInodeToName(iInode, itsName, 512);
	
	/* Print name from root to destination directory. */
	PRIVATE(PrintFullPathTo)(GetInodeNumber("."), lv + 1);
	printf("%s%c", itsName, lv > 0 ? '/' : ' ');
}

/*
 * @brief Entry point function of PrintFullPathTo.
 * Print full path name of given INode number.
 */
void PrintFullPathTo(const ino_t iInode)
{
	PRIVATE(PrintFullPathTo)(iInode, 0);
}

int main(int argc, char** argv)
{
	MACRO_NOTUSED_VARIABLE_BUT_USE_ANYWAY(argc);
	MACRO_NOTUSED_VARIABLE_BUT_USE_ANYWAY(argv);

	PrintFullPathTo(GetInodeNumber("."));
	puts("");
	return 0;
}
