#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cliant.h"

static volatile bool running = true;

static int ping(int argc, char **argv);
static int add(int argc, char **argv);

void sig_handler(int signo)
{
    running = false;
}

int main(int argc, char **argv)
{
    /* Declare CLIant context and register commands */
    struct cliant_ctx cliant_ctx;
    struct cliant_cmd cmds[] = {
        {"ping", &ping, "[NO_ARGUMENTS]", "Print \"ping\""},
        {"add", &add, "--nb1 [an INT] --nb2 [an other INT]", "Add 2 numbers."}
    };

    for (int i = 0; i < sizeof(cmds)/sizeof(struct cliant_cmd); i++)
        if (cliant_cmd_register(&cliant_ctx, cmds[i]))
                fprintf(stderr, "Failed to register %s cmd.\r\n",
                        cmds[i].label);

    /* Create a named pipe */
    int fd;
    char *cliant_fifo = "/tmp/cliant_fifo";

    mkfifo(cliant_fifo, 0666);
    fd = open(cliant_fifo, O_RDONLY);
    if (!fd) {
        fprintf(stderr, "Cannot open cliant fifo.\r\n");
        return -1;
    }

    /* Read commands from fifo */
    char c;
    const size_t buf_size = 255;
    char buf[buf_size];
    size_t buf_n = 0;

    memset(buf, 0, buf_size);

    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    signal(SIGPIPE, sig_handler);
    while(running) {
        if (read(fd, &c, 1)) {
            buf[buf_n] = c;
            buf_n++;
            
            if (c != '\n' && buf_n < buf_size)
                continue;

            if (c == '\n')
                cliant_cmd_parse(&cliant_ctx, buf);

            memset(buf, 0, buf_size);
            buf_n = 0;
        }
    }

    /* Close fifo and clean CLIant context */
    close(fd);
    unlink(cliant_fifo);
    cliant_clean_ctx(&cliant_ctx);
}

static int ping(int argc, char **argv)
{
    if (argc != 1) {
        fprintf(stderr, "USAGE : %s [NO_ARGUMENTS].\r\n", argv[0]);
        return -1;
    }
    fprintf(stdout, "ping\r\n");
    return 0;
}

static int add(int argc, char **argv)
{
    if (argc != 5) {
        fprintf(stderr, "USAGE : %s --nb1 [an INT] --nb2 [an other INT]",
                    argv[0]);
        return -1;
    }

    /* Getopt variables */
    const struct option long_options[] = {
        {"nb1", required_argument, 0, 'm'},
        {"nb2", required_argument, 0, 'n'},
        {NULL, 0, 0, 0}
    };
    int option_index = 0, c = 0;
    optind = 0;

    /* Command specific variables */
    int nb1 = 0, nb2 = 0;

    while((c = getopt_long(argc, argv, "m:n:", long_options, &option_index))
            != -1) {
        switch(c) {
        case 'm':
            nb1 =atoi(optarg);
            break;
        case 'n':
            nb2 = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Issue when parsing add cmd args.\r\n");
            return -1;
        }
    }

    printf("Add %d and %d results to %d.\r\n", nb1, nb2, nb1 + nb2);
    return 0;
}
