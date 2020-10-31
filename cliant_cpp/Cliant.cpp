#include <cstring>
#include <iostream>
#include <cstdlib>

#include "Cliant.hpp"

Cliant::Cliant()
{
}

Cliant::~Cliant()
{
}

void Cliant::printList()
{
    std::cout << "CLIant registered commands :" << std::endl;
    for (auto it = cmds.begin(); it != cmds.end(); ++it)
        std::cout << "=>" << it->label << " " << it->options << std::endl \
            << "        " << it->description << std::endl;
        
}

void Cliant::cmdRegister(struct cliantCmd &cmd)
{
    cmds.push_back(cmd);
    std::cout << "CLIant : " << cmd.label << " cmd registered." << std::endl;
}

int Cliant::cmdCall(std::vector<std::string> &argvStr)
{
    for (auto it = cmds.begin(); it != cmds.end(); it++) {
        if (argvStr[0].find(it->label) != std::string::npos) {
            int (*func) (int argc, char **argv) = it->func;

            int argc = argvStr.size();
            char **argv = (char **) malloc(argc * sizeof(char *));

            for (int i = 0; i < argc; i++) {
                argv[i] = (char *) calloc(argvStr[i].size(), 1);
                memcpy(argv[i], argvStr[i].c_str(), argvStr[i].size());
            }
            func(argc, argv);

            for (int i = 0; i < argc; i++)
                free(argv[i]);
            free(argv);
            return 0;
        }
    }
    std::cerr << "No command corresponds to " << argvStr[0] << std::endl;
    return -1;
}

int Cliant::cmdParse(std::string cmd)
{
    if (cmds.empty()) {
        std::cerr << "No registered commands." << std::endl;
        return -1;
    }

    std::vector<std::string> argvStr;

    int pos = 0;
    for (auto it = cmd.rbegin(); it != cmd.rend(); ++it) {
        if (*it == '\n' || *it == '\r' || *it == ' ') {
            pos++;
        } else {
            cmd = cmd.substr(0, cmd.size() - pos);
            break;
        }
    }

    if (cmd.empty()) {
        std::cerr << "Command line is empty." << std::endl;
        return -1;
    }

    std::string delimiter = " ";

    argvStr.push_back(cmd.substr(0, cmd.find(delimiter)));

    if (cmd.find(delimiter) == std::string::npos) {
        cmd = "";
    } else {
        cmd = cmd.substr(cmd.find(delimiter) + delimiter.length());
    }

    if (argvStr[0].empty()) {
        std::cerr << "No command found." << std::endl;
        return -1;
    }

    if (argvStr[0].find("list") != std::string::npos) {
        printList();
        return 0;
    }

    while (cmd.find(delimiter) != std::string::npos) {
        argvStr.push_back(cmd.substr(0, cmd.find(delimiter)));
        cmd = cmd.substr(cmd.find(delimiter) + delimiter.length());        
    }
    if (!cmd.empty())
        argvStr.push_back(cmd.substr(0, cmd.find(delimiter)));

    if (cmdCall(argvStr)) {
        std::cerr << "Command call failed." << std::endl;
        return -1;
    }
    return 0;
}
