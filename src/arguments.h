#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "help.h"
#include <getopt.h>

static struct option opts[] = {
    { "dir", 1, 0, 'd' },
    { "preset", 1, 0, 'p' }, // TODO: add --build
	{ "list", 0, 0, 'l' },
    { "help",  0, 0, 'h' },
	{ "version", 0, 0, 'v'},
    { 0, 0, 0, 0 }
};

static option_info opts_info[] = {
	{ 0, "set target directory", "DIRECTORY" },
	{ 1, "select preset", "PRESET" },
	{ 2, "list all available presets and exit", 0 },
	{ 3, "display this help and exit", 0 },
	{ 4, "output version information and exit", 0 },
};

#endif
