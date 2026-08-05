#include "status.h"
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

char* status_str(Status status) {
	switch (status) {
		case STATUS_OK: return "ok";
		case STATUS_FAIL: return "error";
		case STATUS_FAIL_OUT_OF_BOUNDS: return "out of bounds error";
		case STATUS_FAIL_IO: return "Io error";
		case STATUS_FAIL_ALLOCATION: return "allocation error";
		default: return "Unknown status";
	}
}

#define MSG_MAX 256

typedef struct {
	FailureLocation location;
	char message[MSG_MAX];
	Status type;
} FailureInfo;

static FailureInfo info = {0};

Status ok() {
	memset(&info, 0, sizeof(FailureInfo));
	return STATUS_OK;
}

Status error(Status type, FailureLocation loc, const char* fmt, ...) {
	info.location = loc;
	info.type = type;
	
	va_list args;
	va_start(args, fmt);
	vsnprintf(info.message, MSG_MAX, fmt, args);
	va_end(args);
	
	return type;
}

Status error_errno(Status type, FailureLocation loc, const char* fmt, ...) {
	char buffer[MSG_MAX];

	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, MSG_MAX, fmt, args);
	va_end(args);

	return error(type, loc, "%s: %s", buffer, strerror(errno));
}

#ifdef _WIN32
#include <windows.h>

Status error_win32(Status type, FailureLocation loc, const char* fmt, ...) {
	DWORD err = GetLastError();

	LPSTR msg = NULL;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		err,
		0,
		(LPSTR)&msg,
		0,
		NULL);

	char buffer[MSG_MAX];

	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, MSG_MAX, fmt, args);
	va_end(args);

	error(type, loc, "WIN32: %s: %s", buffer, msg ? msg : "Unknown error");

	LocalFree(msg);

	return type;
}
#endif

Status status_get() {
	return info.type;
}

bool error_occured() {
	return info.type != STATUS_OK;
}

void error_print() {
	FailureLocation loc = info.location;
	fprintf(stderr, "%s:%d: \033[91m%s:\033[0m %s\n", loc.file, loc.line, status_str(info.type), info.message);
}
