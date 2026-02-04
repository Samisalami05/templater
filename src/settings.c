#include "settings.h"
#include <bits/getopt_core.h>
#include <libgen.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define DEFAULT_PRESET "standard"

/* ------------------ Declarations of internal functions ------------------ */

static uint8_t parse_arguments(settings* s, int argc, char* argv[]);
static uint8_t executable_path(char* exec_path);

/* -------------------------- Internal functions -------------------------- */

static uint8_t parse_arguments(settings* s, int argc, char* argv[]) {
    char relative_path[PATH_MAX - 1];
    getcwd(relative_path, PATH_MAX - 1);

    char path_changed = 0;

    int opt;
    while ((opt = getopt(argc, argv, "d:p:")) != -1) {
        switch (opt) {
        case 'd':
            snprintf(s->path, PATH_MAX, "%s/%s", relative_path, optarg);
            path_changed = 1;
            break;
		case 'p':
			s->preset = optarg;
			break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-d dirname]\n", argv[0]);
            return 1;
        }
    }

	if (argc - optind > 0) {
		fprintf(stderr, "Usage: %s [-d dirname]\n", argv[0]);
		return 1;
	}

    if (!path_changed) { // TODO: what is this???
        snprintf(s->path, PATH_MAX, "%s", relative_path);
    }
	return 0;
}

static uint8_t executable_path(char* exec_path) {
	ssize_t len = readlink("/proc/self/exe", exec_path, PATH_MAX - 1);
    if (len != -1) {
        exec_path[len] = '\0';
		dirname(exec_path); // TODO: is this necessary?
        printf("Executable path: %s\n", exec_path); //TODO: maybe remove
    } else {
        perror("readlink");
		return 1;
    }
	return 0;
}

uint8_t parse_settings(settings* s, int argc, char* argv[]) {
	// Default settings
	s->preset = DEFAULT_PRESET;

	// Parse command-line arguments
    if (parse_arguments(s, argc, argv)) return 1;

	// Retrive executable path
	if (executable_path(s->exec_path)) return 1;
	return 0;
}
