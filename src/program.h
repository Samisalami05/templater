#ifndef PROGRAM_H
#define PROGRAM_H

#define VERSION "1.0.2"

/*
 * VERSION HISTORY
 *
 * 1.0.0 - Initial release
 * 1.0.1 - Added install.sh script and improved the Makefile
 * 1.0.2 - Added command line options
 * 1.0.3 - Added support for Windows and improved error handling
*/

// Settings
#define ENABLE_DEBUG_LOG

#ifdef ENABLE_DEBUG_LOG
#define DEBUG_LOG(...) \
    do { \
        fprintf(stderr, "[DEBUG] %s:%d:%s(): ", __FILE__, __LINE__, __func__); \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
    } while (0)
#else
#define DEBUG_LOG(...) do {} while (0)
#endif



#endif
