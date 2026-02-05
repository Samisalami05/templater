#ifndef PRESET_H
#define PRESET_H

#include <linux/limits.h>
#include <stdint.h>

typedef struct {
	char* name;
	char path[PATH_MAX];
} preset;

uint8_t print_presets();
uint8_t get_preset(preset* p, const char* name);
uint8_t load_preset(const preset p, const char* path);

#endif
