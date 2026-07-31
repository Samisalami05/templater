#ifndef SETTINGS_H
#define SETTINGS_H

#include "preset.h"
#include <linux/limits.h>
#include <stdint.h>

typedef struct {
	char path[PATH_MAX];
	Preset preset;
} Settings;

uint8_t parse_settings(Settings* s, int argc, char* argv[]);


#endif
