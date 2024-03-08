//
// Created by Yulei on 2024/3/8.
//

// STL dependencies
#include <iostream>

// argparse dependencies
#include "argparse/argparse.hpp"

// core dependencies
#include "simulation.h"


enum Color {
    RED,
    BLUE,
    GREEN,
};

struct MyArgs : public argparse::Args {
    Color &color = kwarg("c,color", "An Enum input");
};

int main(int argc, char* argv[]) {
    auto args = argparse::parse<MyArgs>(argc, argv);
    args.print();      // prints all variables

    return 0;
}