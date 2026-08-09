#ifndef FILE_H
#define FILE_H

#include <stdbool.h>

#if defined(__linux__) || defined(__APPLE__)
#include <linux/limits.h>
#include <dirent.h>
#else
#include <windows.h>
#endif

bool file_exists(const char* path);
bool dir_exists(const char* path);
bool is_dir(const char* path);
bool create_dir(const char* path);

#if defined(__linux__) || defined(__APPLE__)
typedef DIR* Dir;
#else 
typedef HANDLE Dir;
#endif

Dir open_dir(const char* path);
char* read_dir(Dir dir);
bool close_dir(Dir dir);

char* get_cwd();
char* get_exe_path();
char* get_exe_dir();

#endif
