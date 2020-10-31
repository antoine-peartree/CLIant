#pragma once

#include <string>
#include <vector>

class Cliant
{
public:
    Cliant();
    ~Cliant();

    struct cliantCmd {
        std::string label;
        int (*func) (int, char**);
        std::string options;
        std::string description;
    };

    void printList();
    void cmdRegister(struct cliantCmd &cmd);
    int cmdCall(std::vector<std::string> &argvStr);
    int cmdParse(std::string cmd);

private:
    std::vector<struct cliantCmd> cmds;
};
