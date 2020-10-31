/*
 * cliant.c
 *
 *  Created on: Oct 28, 2020
 *      Author: antoinepoirier
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cliant.h"

static int cliant_init = false;

/*
 * Init CLIant context.
 */
static void cliant_init_ctx(struct cliant_ctx *ctx) {
	memset(ctx, 0, sizeof(*ctx));
	ctx->cmds = NULL;
}

int cliant_cmd_register(struct cliant_ctx *ctx, struct cliant_cmd cmd) {
	/* Handle first registration */
	if (!cliant_init) {
		cliant_init_ctx(ctx);
		ctx->cmds = malloc(sizeof(struct cliant_cmd));
		if (ctx->cmds == NULL) {
			perror("CLIant cmd register");
			return -errno;
		}
		cliant_init = true;

	/* Handle next registrations */
	} else {
		struct cliant_cmd *tmp_cmds =
				realloc(ctx->cmds,
						(ctx->nb_registered_cmds + 1) * sizeof(struct cliant_cmd));
		if (tmp_cmds == NULL) {
			perror("CLIant cmd register");
			return -errno;
		}
		ctx->cmds = tmp_cmds;
	}

	memcpy(&ctx->cmds[ctx->nb_registered_cmds], &cmd,
			sizeof(struct cliant_cmd));
	ctx->nb_registered_cmds++;

	printf("CLIant: Command %s registered.\r\n", cmd.label);
	return 0;
}

/*
 * Print a list of available registered commands.
 */
static void print_list(struct cliant_ctx *ctx)
{
	printf("CLIant registered commands :\r\n");
	for (int i = 0; i < ctx->nb_registered_cmds; i++)
		printf("=> %s %s\r\n        %s\r\n",
				ctx->cmds[i].label,
				ctx->cmds[i].options,
				ctx->cmds[i].description);
}

/*
 * Use argv[0] to find the corresponding cmd into the registered cmds list.
 */
static int cmd_call(struct cliant_ctx *ctx, int argc, char **argv) {
	if (!ctx->nb_registered_cmds) {
		fprintf(stderr, "CLIant: No command registered, aborting cmd call.\r\n");
		return -1;
	}

	for (int i = 0; i < ctx->nb_registered_cmds; i++) {
		if (!strcmp(ctx->cmds[i].label, argv[0])) {
			typedef int func(int argc, char **argv);
			func *f = ctx->cmds[i].func;
			f(argc, argv);
			return 0;
		}
	}

	fprintf(stderr, "CLIant: No command corresponds to %s.\r\n", argv[0]);
	return -1;
}

int cliant_cmd_parse(struct cliant_ctx *ctx, char *cmd) {
	int res;
	int argc = 1;
	char *argv[128];

	if (cmd == NULL) {
		fprintf(stderr, "CLIant: Command line pointer is NULL.\r\n");
		return -1;
	}

	/* Cut possible end-of-string chars or trailing whitespaces */
	for (int i = strlen(cmd) - 1; i > 0; i++) {
		if (cmd[i] == '\n' || cmd[i] == '\r' || cmd[i] == ' ') {
			cmd[i] = '\0';
		} else {
			break;
		}
	}

	if (!strlen(cmd)) {
		fprintf(stderr, "CLIant: Command line is empty.\r\n");
		return -1;
	}

	argv[0] = strtok(cmd, " ");
	if (argv[0] == NULL) {
		fprintf(stderr, "CLIant: Cannot find a command into command line.\r\n");
		return -1;
	}

	if (!strcmp(argv[0], "list")) {
		print_list(ctx);
		return 0;
	}

	char *tmp_arg;
	do {
		tmp_arg = strtok(NULL, " ");
		if (tmp_arg == NULL)
			break;

		argv[argc] = tmp_arg;
		argc++;
	} while (tmp_arg != NULL);

	res = cmd_call(ctx, argc, argv);
	if (res) {
		fprintf(stderr, "CLIant: Command call failed. \r\n");
		return res;
	}

	return 0;
}

void cliant_clean_ctx(struct cliant_ctx *ctx) {
	free(ctx->cmds);
	memset(ctx, 0, sizeof(*ctx));
	ctx->cmds = NULL;
	cliant_init = false;
}
