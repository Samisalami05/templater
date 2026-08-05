#ifndef PRESET_H
#define PRESET_H

#include <stdint.h>
#include "file.h"

typedef struct {
	char* name;
	char path[PATH_MAX];
} Preset;

uint8_t print_presets();
uint8_t get_preset(Preset* p, const char* name);
uint8_t load_preset(const Preset p, const char* path);

#endif
