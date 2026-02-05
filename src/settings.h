#ifndef SETTINGS_H
#define SETTINGS_H

#include "preset.h"
#include <linux/limits.h>
#include <stdint.h>

typedef struct {
	char path[PATH_MAX];
	preset preset;
} settings;

uint8_t parse_settings(settings* s, int argc, char* argv[]);


#endif
