#include "preset.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "status.h"
#include "program.h"

static uint8_t copy_file(const char* dest, const char* source) {
	FILE* source_fp = fopen(source, "rb");
	if (source_fp == NULL) {
		perror("copy source fopen");
		return 1;
	}

	FILE* dest_fp = fopen(dest, "wb");
	if (dest_fp == NULL) {
		perror("copy dest fopen");
		fclose(source_fp);
		return 1;
	}

	fseek(source_fp, 0, SEEK_END);
	unsigned long size = ftell(source_fp);
	fseek(source_fp, 0, SEEK_SET);

	uint8_t* data = malloc(size);
	fread(data, size, 1, source_fp);
	fwrite(data, size, 1, dest_fp);
	free(data);

	fclose(source_fp);
	fclose(dest_fp);
	return 0;
}

static uint8_t copy_dir_contents(const char* dest, const char* source) {
	Dir dir = open_dir(source);
	if (!dir) return 1;

	char* entry = NULL;
	while ((entry = read_dir(dir)) != NULL) {
		if (strcmp(entry, ".") == 0 || strcmp(entry, "..") == 0)
			continue;

		char path[PATH_MAX];
		snprintf(path, PATH_MAX, "%s/%s", source, entry);
		
		char destination[PATH_MAX];
		snprintf(destination, PATH_MAX, "%s/%s", dest, entry);

		if (is_dir(path)) { // Check if it is a directory
			printf("copying dir:  %s\n", entry);
			create_dir(destination);
			if (copy_dir_contents(destination, path)) return 1;
		}
		else {
			printf("copying file: %s\n", entry);
			if (copy_file(destination, path)) return 1;
		}
			
	}
	close_dir(dir);
	return 0;
}

static uint8_t print_dir_contents(Dir dir) {
	char* entry = NULL;
	while ((entry = read_dir(dir)) != NULL) {
		if (strcmp(entry, ".") == 0 || strcmp(entry, "..") == 0)
			continue;
		printf(" - %s\n", entry);		
	}

	return 0;
}

static void get_presets_path(char path[PATH_MAX]) {
#if defined (__linux__) || defined(__APPLE__)
	const char* home = getenv("HOME");
	snprintf(path, PATH_MAX, "%s/.local/share/templater/presets", home);
#else
	const char* local = getenv("LocalAppData");
	snprintf(path, PATH_MAX, "%s/templater/presets", local);
#endif
}

uint8_t print_presets() {
	char presets_path[PATH_MAX];
	get_presets_path(presets_path);

	Dir dir = open_dir(presets_path);
	if (!dir) {
		error_print();
		return 1;
	}

	printf("\nAvailable presets\n");
	print_dir_contents(dir);
	printf("\n");

	close_dir(dir);

	return 0;
}

uint8_t get_preset(Preset* p, const char* name) {
	char presets_path[PATH_MAX];
	get_presets_path(presets_path);

	if (!dir_exists(presets_path)) {
		error_print();
		fprintf(stderr, "Could not find the presets directory: '%s'\n", presets_path);
		return 1;
	}

	snprintf(p->path, PATH_MAX, "%s/%s", presets_path, p->name);

	Dir preset_dir = open_dir(p->path); // TODO: Maybe store this in the preset struct
	if (!preset_dir) {
		error_print();
		fprintf(stderr, "Could not find the requested preset: '%s'\n", p->name);

		printf("\nAvailable presets\n");
		print_dir_contents(preset_dir);
		printf("\n");

		return 1;
	}

	close_dir(preset_dir);

	return 0;
}

uint8_t load_preset(Preset p, const char* path) {
	if (copy_dir_contents(path, p.path)) {
		fprintf(stderr, "failed to copy files\n");
		return 1;
	}
	return 0;
}
