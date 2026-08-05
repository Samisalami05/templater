#ifndef STATUS_H
#define STATUS_H

#include <stdbool.h>

typedef enum {
	STATUS_OK,
	STATUS_FAIL,
	STATUS_FAIL_OUT_OF_BOUNDS,
	STATUS_FAIL_ALLOCATION,
	STATUS_FAIL_IO,
} Status;

typedef struct {
	const char* file;
	const char* func;
	int line;
} FailureLocation;

Status status_get();
bool error_occured();

char* status_str(Status status);

Status ok();

Status error(Status type, FailureLocation loc, const char* fmt, ...);
#define ERROR(type, fmt, ...) error(type, (FailureLocation){__FILE__, __func__, __LINE__}, fmt, ##__VA_ARGS__)

Status error_errno(Status type, FailureLocation loc, const char* fmt, ...);
#define PERROR(type, fmt, ...) error_errno(type, (FailureLocation){__FILE__, __func__, __LINE__}, fmt, ##__VA_ARGS__)

#ifdef _WIN32
Status error_win32(Status type, FailureLocation loc, const char* fmt, ...);
#define PERROR_WIN32(type, fmt, ...) error_win32(type, (FailureLocation){__FILE__, __func__, __LINE__}, fmt, ##__VA_ARGS__)
#endif

void error_print();

#endif