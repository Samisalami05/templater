#include "file.h"
#include <stdio.h>
#include <stdbool.h>
#include "status.h"

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#else
#include <windows.h>
#endif


bool file_exists(const char* path) {
#if defined(__linux__) || defined(__APPLE__)
    if (access(path, F_OK) == 0) {
        return true;
    }
    PERROR(STATUS_FAIL_IO, "Failed to check if file exists: '%s'", path);
    return false;
#endif

    FILE* file = fopen(path, "r");
    if (file) {
        fclose(file);
        return true;
    }
    PERROR(STATUS_FAIL_IO, "Failed to check if file exists: '%s'", path);
    return false;
}

bool dir_exists(const char* path) {
#if defined(__linux__) || defined(__APPLE__)
    if (access(path, F_OK) == 0) {
        return true;
    }
    PERROR(STATUS_FAIL_IO, "Failed to check if directory exists: '%s'", path);
    return false;
#else
    DWORD attributes = GetFileAttributesA(path);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        PERROR_WIN32(STATUS_FAIL_IO, "Failed to get file attributes for '%s'", path);
        return false;
    }
    return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
#endif
}

bool is_dir(const char* path) {
#if defined(__linux__) || defined(__APPLE__)
    struct stat info;
    if (lstat(path, &info) != 0) {
        PERROR(STATUS_FAIL_IO, "Failed to get file status for '%s'", path);
        return false;
    }
    return (info.st_mode & S_IFMT) == S_IFDIR;
#else
    DWORD attributes = GetFileAttributesA(path);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        PERROR_WIN32(STATUS_FAIL_IO, "Failed to get file attributes for '%s'", path);
        return false;
    }
    return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
#endif
}

// Returns true if the directory was created successfully, false otherwise
bool create_dir(const char* path) {
#if defined(__linux__) || defined(__APPLE__)
    if (mkdir(path, 0777) != 0) {
        PERROR(STATUS_FAIL_IO, "Failed to create directory '%s'", path);
        return false;
    }
    return true;
#else
    if (!CreateDirectoryA(path, NULL)) {
        PERROR_WIN32(STATUS_FAIL_IO, "Failed to create directory '%s'", path);
        return false;
    }
    return true;
#endif
}

char name[PATH_MAX] = ""; // Buffer used by file functions

#if defined(_WIN32)
bool is_unused = false;
#endif

// Returns null on error and status is set to indicate the error.
Dir open_dir(const char* path) {
#if defined(__linux__) || defined(__APPLE__)
    Dir dir = opendir(path);
    if (!dir) {
        PERROR(STATUS_FAIL_IO, "Failed to open directory '%s'", path);
        return NULL;
    }
    return dir;
#else
    char search_path[PATH_MAX];
    snprintf(search_path, PATH_MAX, "%s\\*", path);

    WIN32_FIND_DATAA find_data;
    Dir dir = FindFirstFileA(search_path, &find_data);
    if (dir == INVALID_HANDLE_VALUE) { 
        PERROR_WIN32(STATUS_FAIL_IO, "Failed to open directory '%s'", path);
        return NULL;
    }

    strncpy(name, find_data.cFileName, PATH_MAX);
    is_unused = true;
    return dir;
#endif
}

char* read_dir(Dir dir) {
#if defined(__linux__) || defined(__APPLE__)
    struct dirent *entry = readdir(dir);
    while (entry != NULL && (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)) {
        entry = readdir(dir);
    }
    if (entry == NULL) {
        PERROR(STATUS_FAIL_IO, "Failed to read directory");
        return NULL;
    }
    return entry->d_name;
#else
    if (is_unused) {
        is_unused = false;
        return name;
    }

    WIN32_FIND_DATAA find_data;
    if (!FindNextFileA(dir, &find_data)) {
        if (GetLastError() != 0) {
            PERROR_WIN32(STATUS_FAIL_IO, "Failed to read directory");
        }
        return NULL;
    }
    strncpy(name, find_data.cFileName, PATH_MAX);
    return name;
#endif
}

bool close_dir(Dir dir) {
#if defined(__linux__) || defined(__APPLE__)
    if (closedir(dir) != 0) {
        PERROR(STATUS_FAIL_IO, "Failed to close directory");
        return false;
    }
    return true;
#else
    if (!FindClose(dir)) {
        PERROR_WIN32(STATUS_FAIL_IO, "Failed to close directory");
        return false;
    }
    return true;
#endif
}

char* get_cwd() {
#if defined(__linux__) || defined(__APPLE__)
    if (getcwd(name, PATH_MAX) != 0) {
        PERROR(STATUS_FAIL_IO, "Failed to get current working directory");
        return NULL;
    }
    return name;
#else
    if (!GetCurrentDirectoryA(PATH_MAX, name)) {
        PERROR_WIN32(STATUS_FAIL_IO, "Failed to get current working directory");
        return NULL;
    }
    return name;
#endif
}

char* get_exe_path() {
#if defined(__linux__) || defined(__APPLE__)
    // TODO: this might not work on all unix systems
    if (readlink("/proc/self/exe", name, PATH_MAX) == -1) {
        PERROR(STATUS_FAIL_IO, "Failed to get executable path");
        return NULL;
    }
    return name;
#else
    if (!GetModuleFileNameA(NULL, name, PATH_MAX)) {
        PERROR_WIN32(STATUS_FAIL_IO, "Failed to get executable path");
        return NULL;
    }
    return name;
#endif
}

char* get_exe_dir() {
    char* exe_path = get_exe_path();
    if (!exe_path) {
        return NULL;
    }

    char* last_slash = strrchr(exe_path, '/');
    if (!last_slash) {
        last_slash = strrchr(exe_path, '\\');
    }

    if (last_slash) {
        *last_slash = '\0'; // Terminate the string at the last slash
    } else {
        ERROR(STATUS_FAIL_IO, "Failed to determine executable directory");
        return NULL;
    }

    return exe_path;
}

