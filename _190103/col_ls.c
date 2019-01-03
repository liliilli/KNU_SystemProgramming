/*
 * @file : col_ls.c
 * @author : 2012104208_Jongmin Yun
 * @date : 2018-01-03 14:15 Created
 *         2018-01-03 22:51 Add comments
 * @description : Display entries of given directory, doing alignment size
 * of entries like a `ls directory`
 * 
 * Usage : `./col_ls` or `./col_ls directory_path`
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <dirent.h>

/*
 * @brief Process files and get attribute, print them.
 */
void ProcessLs(const char* iDirName);

/*
 * @brief Get size of screen dimension.
 */
void GetPrintScreenSize(unsigned int* row, unsigned int* col);

int main(int argc, char** argv)
{
	int i;

	if (argc == 1) { ProcessLs("."); }
	else
	{
		for (i = 1; i < argc; ++i)
		{
			printf("%s: \n", argv[i]);
			ProcessLs(argv[i]);
		}
	}
	return 0;
}

void ProcessLs(const char* iDirName)
{
	/* Check dir is exist */
	DIR* ptrDir = NULL;
	if ((ptrDir = opendir(iDirName)) == NULL)
	{
		fprintf(stderr, "ls1 : Failed to open directory %s\n", iDirName);
		return;
	}
	
	/* Get window consle row & column size */ 
	unsigned winCol, winRow;
	GetPrintScreenSize(&winRow, &winCol);

	/* Get `alphabet sorted` entry list of `iDirName` directory. */
	struct dirent** entryList = NULL;
	int entryCount = scandir(iDirName, &entryList, NULL, alphasort);
	if (entryCount < 0) { perror("scandir"); }
	else
	{
		int i = 0;
		while (i < entryCount) { printf("%s\n", entryList[i++]->d_name); }
	}

	/* Calculate column item length. */
	int maxColItem = 1;
	int* colMaxLenList = (int*)malloc(entryCount * sizeof(int));
	int outFlag = 0;
	do
	{
		memset(colMaxLenList, 0, sizeof(int) * entryCount);
		/* Get max size of given maxColItem */
		for (int i = 0; i < maxColItem; ++i)
		{	
			for (int j = i; j < entryCount; j += maxColItem)
			{
				int len = strlen(entryList[j]->d_name) + 3;
				if (colMaxLenList[i] < len) { colMaxLenList[i] = len; }
			}
		}

		unsigned sumLength = 0;
		for (int i = 0; i < entryCount; ++i) { sumLength += colMaxLenList[i]; }

		if (outFlag == 1) { break; }
		if (sumLength > winCol) 
		{ 
			if (maxColItem == 1) { break; }
			maxColItem -= 1;
			outFlag = 1; 
		}

		if (maxColItem == entryCount) { break; } else { maxColItem += 1; }
	}
	while (1);
	
	/* Print Sequence */
	for (int i = 0; i < entryCount; ++i)
	{
		/* Make meta string format of each column. */
		char strFormat[16] = {'\0', };
		strcat(strFormat, "%-");
		char intStr[16] = {'\0', };
		sprintf(intStr, "%d", colMaxLenList[i % maxColItem] - 3);
		strcat(strFormat, intStr);
		strcat(strFormat, "s ");

		/* Print */
		printf(strFormat, entryList[i]->d_name);
		if ((i + 1) % maxColItem == 0) { printf("\n"); }
	}

	/* Free entryList */
	int i = 0; 
	while (i < entryCount) { free(entryList[i++]); }
	free(entryList);
	free(colMaxLenList);

	int closeFlag = closedir(ptrDir);
	if (closeFlag == -1)
	{
		fprintf(stderr, "ls1 : Failed to close directory sturcture instance %s\n", iDirName);
	}
}

void GetPrintScreenSize(unsigned int* row, unsigned int* col)
{
	struct winsize sizeBuffer;
	if (ioctl(0, TIOCGWINSZ, &sizeBuffer) == -1)
	{
		*row = 0;
		*col = 0;
		return;
	}
	
	*row = sizeBuffer.ws_row;
	*col = sizeBuffer.ws_col;
}
