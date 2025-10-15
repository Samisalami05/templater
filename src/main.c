#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <linux/limits.h>
#include <libgen.h>

static void parse_arguments(int argc, char* argv[], char* path);
static void create_folders(char* relative_path, char** folders);
static void create_file(char* exe_path, char* relative_path, char* read_path, char* write_path);

int main(int argc, char* argv[]) {
	// Parse command-line arguments
    char path[PATH_MAX];
    parse_arguments(argc, argv, path);

	// Retrive executable path
	char exe_path[PATH_MAX];
	ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len != -1) {
        exe_path[len] = '\0';
		dirname(exe_path);
        printf("Executable path: %s\n", exe_path);
    } else {
        perror("readlink");
		exit(EXIT_FAILURE);
    }

    // Create directory if it does not exist
    if (access(path, F_OK) != 0) {
        mkdir(path, 0777);
    }

    printf("path: %s\n", path);

    char* folders[] = {"src", "build", "libs", NULL};
    create_folders(path, folders);
    create_file(exe_path, path, "template_files/t_Makefile", "Makefile");
	create_file(exe_path, path, "template_files/t_.gitignore", ".gitignore");
	create_file(exe_path, path, "template_files/t_main.c", "src/main.c");

    return 0;
}

static void parse_arguments(int argc, char* argv[], char* path) {
    char relative_path[PATH_MAX - 1];
    getcwd(relative_path, PATH_MAX - 1);

    char path_changed = 0;

    int opt;
    while ((opt = getopt(argc, argv, "d:")) != -1) {
        switch (opt) {
        case 'd':
            snprintf(path, PATH_MAX, "%s/%s", relative_path, optarg);
            path_changed = 1;
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-d dirname]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (!path_changed) {
        snprintf(path, PATH_MAX, "%s", relative_path);
    }
}

static void create_folders(char* relative_path, char** folders) {
    int i = 0;
    while (folders[i] != NULL) {
		char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s/%s", relative_path, folders[i]);
        if (access(path, F_OK) != 0) {
            mkdir(path, 0777);
            printf("created folder: %s\n", folders[i]);
        }
		else {
			printf("folder already exits: %s\n", folders[i]);
		}
        i++;
    }
}

static void create_file(char* exe_path, char* relative_path, char* read, char* write) {
    char read_path[PATH_MAX];
    char write_path[PATH_MAX];

    snprintf(read_path, PATH_MAX, "%s/%s", exe_path, read);
    snprintf(write_path, PATH_MAX, "%s/%s", relative_path, write);

    FILE* r_fp = fopen(read_path, "r");
    FILE* w_fp = fopen(write_path, "w");

    if (r_fp == NULL) {
        fprintf(stderr, "templater: Cant open file %s: %s\n", read_path, strerror(2));
        return;
    }
	if (w_fp == NULL) {
        fprintf(stderr, "templater: Cant open file %s: %s\n", write_path, strerror(2));
		fclose(r_fp);
        return;
    }
    
    char c;
    while ((c = fgetc(r_fp)) != EOF) {
        fputc(c, w_fp);
    }

    fclose(r_fp);
    fclose(w_fp);

	printf("Created file: %s\n", write);
}