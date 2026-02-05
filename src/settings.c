#include "settings.h"
#include <getopt.h>
#include <libgen.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "help.h"
#include "arguments.h"

#define DEFAULT_PRESET "standard"

/* ------------------ Declarations of internal functions ------------------ */

static uint8_t parse_arguments(settings* s, int argc, char* argv[]);

/* -------------------------- Internal functions -------------------------- */

static uint8_t parse_arguments(settings* s, int argc, char* argv[]) {
    char relative_path[PATH_MAX];
	getcwd(relative_path, PATH_MAX - 1);
	strcpy(s->path, relative_path);

    int opt;
    while ((opt = getopt_long(argc, argv, "d:p:lhv", opts, NULL)) != -1) {
        switch (opt) {

        case 'd':
            snprintf(s->path, PATH_MAX, "%s/%s", relative_path, optarg);
            break;
		case 'p':
			s->preset.name = optarg;
			break;
		case 'l':
			print_presets();
			return 1;
		case 'h':
			print_help(argv[0]);
			return 1; // TODO: maybe should return 0
		case 'v':
			print_version();
			return 1;
        default:
			print_usage(argv[0]);
            return 1;
        }
    }

	if (argc - optind > 0) {
		print_usage(argv[0]);
		return 1;
	}

	return 0;
}

uint8_t parse_settings(settings* s, int argc, char* argv[]) {
	// Default settings
	s->preset.name = DEFAULT_PRESET;

    if (parse_arguments(s, argc, argv)) return 1;
	if (get_preset(&s->preset, s->preset.name)) return 1;

	return 0;
}
