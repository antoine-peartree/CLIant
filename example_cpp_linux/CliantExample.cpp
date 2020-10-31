#include <fcntl.h>
#include <getopt.h>
#include <iostream>
#include <csignal>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include "CliantExample.hpp"

static volatile bool running = true; 

static int ping(int argc, char **argv);
static int add(int argc, char **argv);

static void sigHandler(int siginfo) { running = false; }

CliantExample::CliantExample()
{
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
    signal(SIGPIPE, sigHandler);
}

CliantExample::~CliantExample()
{
}

int CliantExample::run(void)
{
    std::vector<Cliant::cliantCmd> cmds = {
        {"ping", &ping, "[NO_ARGUMENTS]", "Print \"ping\""},
        {"add", &add, " --nb1 [an INT] --nb2 [an other INT]", "Add two numbers"}
    };
    for (auto it = cmds.begin(); it != cmds.end(); ++it)
        cliant.cmdRegister(*it);

    /* Create a named pipe */
    int fd;
    std::string cliantFifo = "/tmp/cliant_fifo";

    mkfifo(cliantFifo.c_str(), 0666);
    fd = open(cliantFifo.c_str(), O_RDONLY);
    if (!fd) {
        std::cerr << "Cannot open cliant fifo." << std::endl;
        return -1;
    }

    /* Read commands from fifo */
    char c;
    std::string buf;

    while (running) {
        if (read(fd, &c, 1)) {
            buf.push_back(c);
            if (c != '\n')
                continue;

            cliant.cmdParse(buf.c_str());
            buf.clear();
        }
    }

    /* Close fifo and clean CLIant context */
    close(fd);
    unlink(cliantFifo.c_str());
    return 0;
}

static int ping(int argc, char **argv)
{
    if (argc != 1) {
        std::cerr << argv[0] << "[NO_ARGUMENTS]" << std::endl;
        return -1;
    }

    std::cout << "ping" << std::endl;
    return 0;
}

static int add(int argc, char **argv)
{
    if (argc != 5) {
        std::cerr << "USAGE : " << argv[0] << \
            " --nb1 [an INT] --nb2 [an other INT]" << std::endl;
        return -1;
    }

    /* Getopt variables */
    const struct option longOptions[] = {
        {"nb1", required_argument, 0, 'm'},
        {"nb2", required_argument, 0, 'n'},
        {NULL, 0, 0, 0}
    };
    int optionIndex = 0, c = 0;
    optind = 0;

    /* Command specific variables */
    int nb1 = 0, nb2 = 0;

    while((c = getopt_long(argc, argv, "m:n:", longOptions, &optionIndex))
            != -1) {
        switch(c) {
        case 'm':
            nb1 =atoi(optarg);
            break;
        case 'n':
            nb2 = atoi(optarg);
            break;
        default:
            std::cerr << "Issue when parsing add cmd args.\r\n" << std::endl;
            return -1;
        }
    }

    std::cout << "Add " << nb1 << " and " << nb2 << " results to " << \
        nb1 + nb2 << std::endl;
    return 0;
}
