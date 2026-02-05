#include "preset.h"
#include "settings.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <libgen.h>
#include <dirent.h>

int main(int argc, char* argv[]) {
	settings s;
	if (parse_settings(&s, argc, argv)) return 1;

	printf("\nSelected preset: %s\n", s.preset.name);
    printf("Path: %s\n", s.path);

	printf("\n:: Proceed with project creation? [Y/n]");

	char choice;
	scanf("%c", &choice);

	if (choice != 'y' && choice != 'Y' && choice != '\n') return 0;

	// Create directory if it does not exist
    if (access(s.path, F_OK) != 0) {
        mkdir(s.path, 0777);
    }

	if (load_preset(s.preset, s.path)) return 1;
 
	return 0;
}

