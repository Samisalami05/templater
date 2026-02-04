#include "preset.h"
#include <dirent.h>
#include <linux/limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static uint8_t copy_file(char* dest, char* source) {
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

	uint8_t data[size];
	fread(data, size, 1, source_fp);
	fwrite(data, size, 1, dest_fp);

	fclose(source_fp);
	fclose(dest_fp);
	return 0;
}

static uint8_t copy_dir_contents(char* dest, char* source) {
	DIR* dir = opendir(source);
	if (!dir) {
		perror("opendir");
		return 1;
	}

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		char path[PATH_MAX];
		snprintf(path, PATH_MAX, "%s/%s", source, entry->d_name);
		
		char destination[PATH_MAX];
		snprintf(destination, PATH_MAX, "%s/%s", dest, entry->d_name);

		struct stat info;
		lstat(path, &info);
		if ((info.st_mode & S_IFMT) == S_IFDIR) { // Check if it is a directory
			printf("copying dir: %s\n", entry->d_name);
			mkdir(destination, 0777);
			if (copy_dir_contents(destination, path)) return 1;
		}
		else {
			printf("copying file: %s\n", entry->d_name);
			if (copy_file(destination, path)) return 1;
		}
			
	}
	closedir(dir);
	return 0;
}

uint8_t print_presets(settings* s) {
	char presets_path[PATH_MAX];
	snprintf(presets_path, PATH_MAX, "%s/presets", s->exec_path);
	DIR* dir = opendir(presets_path);
	if (!dir) {
		perror("opendir");
		return 1;
	}

	printf("\nAvailable presets\n");

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		printf(" - %s\n", entry->d_name);		
	}
	closedir(dir);

	return 0;
}

uint8_t load_preset(settings* s) {
	char preset_path[PATH_MAX];
	snprintf(preset_path, PATH_MAX, "%s/presets/%s", s->exec_path, s->preset);

	DIR* dir = opendir(preset_path);
	if (!dir) {
		perror("Could not access the requested preset");
		print_presets(s);
		return 1;
	}
	closedir(dir);
	
	if (copy_dir_contents(s->path, preset_path)) {
		fprintf(stderr, "failed to copy files\n");
		return 1;
	}
	return 0;
}
