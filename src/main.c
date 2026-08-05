#include "preset.h"
#include "settings.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "status.h"


int main(int argc, char* argv[]) {
	
	Settings s = {0};
	if (parse_settings(&s, argc, argv)) return 1;

	printf("\nSelected preset: %s\n", s.preset.name);
    printf("Path: %s\n", s.path);

	printf("\n:: Proceed with project creation? [Y/n]");

	char choice = fgetc(stdin);

	if (choice != 'y' && choice != 'Y' && choice != '\n') return 0;

	// Create directory if it does not exist
    if (!dir_exists(s.path)) {
        if (!create_dir(s.path)) {
            error_print();
            return 1;
        }
    }

	if (load_preset(s.preset, s.path)) return 1;
 
	return 0;
}

