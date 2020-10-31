#pragma once

#include "Cliant.hpp"


class CliantExample
{
public:
    ~CliantExample();

    static CliantExample &getInstance()
    {
        static CliantExample c;
        return c;
    }

    int run();

private:
    CliantExample();

    Cliant cliant;
};
