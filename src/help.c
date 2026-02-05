#include "help.h"
#include "arguments.h"
#include "program.h"
#include <stdio.h>
#include <string.h>

void print_usage(const char* prog_name) {
	fprintf(stderr, "Usage: %s [OPTION...]\n", prog_name);
}

void print_option(option_info info) {
	struct option o = opts[info.opt_index];
	if (o.has_arg) {
		int len = strlen(o.name) + strlen(info.argument) + 2;
		char combined[len];
		snprintf(combined, len, "%s=%s", o.name, info.argument);
		printf("  -%c, --%-16s %s\n", o.val, combined, info.description);
	} else
		printf("  -%c, --%-16s %s\n", o.val, o.name, info.description);

}

void print_options() {
	struct option* opt = opts;
	option_info* info = opts_info;

	while (opt->name != NULL) {
		print_option(*info);

		opt++;
		info++;
	}
}

void print_help(const char* prog_name) {
	print_usage(prog_name);
	printf("Create template project from PRESET to DIRECTORY\n\n");
	print_options();

	printf("\nA PRESET is a template and DIRECTORY is the directory where the\nproject files should be generated.\n");
	printf("\nBy default, PRESET is set to 'standard' and the DIRECTORY is set to\nthe location the program is executed from.\n");
	printf("\nIf the given DIRECTORY does not exist, it will be created by the\nprogram before the project files are copied over.\n");
}

void print_version() {
	printf("version %s\n", VERSION);

	printf("\nMade by Sam Gustafsson.\n");
}
