/*
 * cliant.h
 *
 *  Created on: Oct 28, 2020
 *      Author: Antoine Poirier
 *
 *      CLIant is a compact lib for bare-metal embedded software.
 *
 *      It allows developers to use command lines (Shell-like) received by serial
 *      to call dedicated functions with command line options as arguments.
 *
 *      You can then have several myFunction(int argc, char **argv) on your system
 *      for easy testing.
 */

#pragma once

/*
 * CLIant command.
 * 	- Label refer to the program name in a shell-like cmd line (= argv[0])
 * 	- Func is a pointer to the function associated with the command
 * 	- Options description shows argv[1-]. (No other use that in usage())
 * 	- Description is a global description of the command.
 */
struct cliant_cmd {
	char *label;
	void *func;
	char *options;
	char *description;
};

/*
 * CLIant context.
 */
struct cliant_ctx {
	struct cliant_cmd *cmds;
	size_t nb_registered_cmds;
};

/*
 * Register a new command.
 */
int cliant_cmd_register(struct cliant_ctx *ctx, struct cliant_cmd cmd);

/*
 * Parse a command into argc and argv, then call a corresponding function with these args.
 */
int cliant_cmd_parse(struct cliant_ctx *ctx, char *cmd);

/*
 * Clean CLIant context.
 * Context should be init again when using CLIant later.
 */
void cliant_clean_ctx(struct cliant_ctx *ctx);
