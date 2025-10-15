#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#define PATH_MAX 4096

static void create_folders(char* relative_path, char** folders);

int main(void) {
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);

	char* folders[] = {"src", "build", "libs", NULL};
	create_folders(path, folders);
	return 0;
}

static void create_folders(char* relative_path, char** folders) {
	int i = 0;
	while (folders[i] != NULL) {
		if (access(folders[i], F_OK) != 0) {
			char path[PATH_MAX];
			snprintf(path, PATH_MAX, "%s/%s", relative_path, folders[i]);

			mkdir(path, 0777);
			printf("created folder: %s\n", folders[i]);
		}
		i++;
	}
}
	
