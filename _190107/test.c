#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

int main()
{
	DIR* dir = opendir(".");
	if (dir == NULL) { exit(1); }

	struct dirent* dirent;
	struct stat statstr;
	while ((dirent = readdir(dir)) != NULL)
	{
		if (stat(dirent->d_name, &statstr) != -1)
		{
			printf("%o %s\n", statstr.st_mode, getpwuid(statstr.st_uid)->pw_name);
		}
	}
	closedir(dir);
	return 0;
}
