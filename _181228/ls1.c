/*
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

/*
 * @brief Process files and get attribute, print them.
 */
void ProcessLs(const char* iDirName);

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
}

void ProcessLs(const char* iDirName)
{
	DIR* ptrDir = NULL;
	struct dirent* ptrDirent = NULL;
	int closeFlag = 0;

	if ((ptrDir = opendir(iDirName)) == NULL)
	{
		fprintf(stderr, "ls1 : Failed to open directory %s\n", iDirName);
		return;
	}

	while ((ptrDirent = readdir(ptrDir)) != NULL)
	{
		printf("%s\n", ptrDirent->d_name);
	}
	
	closeFlag = closedir(ptrDir);
	if (closeFlag == -1)
	{
		fprintf(stderr, "ls1 : Failed to close directory sturcture instance %s\n", iDirName);
	}
}
