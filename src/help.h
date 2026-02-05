#ifndef HELP_H
#define HELP_H

#include <getopt.h>

typedef struct {
	int opt_index;
	char* description;
	char* argument;
} option_info;

void print_usage(const char* prog_name);
void print_help(const char* prog_name);
void print_version();

#endif
