#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define PATH_MAX 4096

static void parse_arguments(int argc, char* argv[], char* path);
static void create_folders(char* relative_path, char** folders);
static void create_file(char* relative_path, char* read_path, char* write_path);

int main(int argc, char* argv[]) {
    char path[PATH_MAX];
    parse_arguments(argc, argv, path);

    // Create directory if it does not exist
    if (access(path, F_OK) != 0) {
        mkdir(path, 0777);
    }

    printf("path: %s\n", path);

    char* folders[] = {"src", "build", "libs", NULL};
    create_folders(path, folders);
    create_file(path, "Makefile", "CreatedMakeFile");

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
        if (access(folders[i], F_OK) != 0) {
            char path[PATH_MAX];
            snprintf(path, PATH_MAX, "%s/%s", relative_path, folders[i]);

            mkdir(path, 0777);
            printf("created folder: %s\n", folders[i]);
        }
        i++;
    }
}

static void create_file(char* relative_path, char* read, char* write) {
    char read_path[PATH_MAX];
    char write_path[PATH_MAX];

    snprintf(read_path, PATH_MAX, "%s/%s", relative_path, read);
    snprintf(write_path, PATH_MAX, "%s/%s", relative_path, write);


    FILE* r_fp = fopen(read_path, "r");
    FILE* w_fp = fopen(write_path, "w");

    if (r_fp == NULL) {
        fprintf(stderr, "templater: Cant open file %s: %s\n", read_path, strerror(2));
        return;
    }
    
    char c;
    while ((c = fgetc(r_fp)) != EOF) {
        fputc(c, w_fp);
    }

    fclose(r_fp);
    fclose(w_fp);
}