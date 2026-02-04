#ifndef SETTINGS_H
#define SETTINGS_H

#include <linux/limits.h>
#include <stdint.h>

typedef struct {
	char path[PATH_MAX];
	char exec_path[PATH_MAX];
	char* preset;
} settings;

uint8_t parse_settings(settings* s, int argc, char* argv[]);


#endif
