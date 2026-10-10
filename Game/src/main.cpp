#include "Game/Game.hpp"
#include <tul/CliOps.hpp>

// Globals
int width = 1920;
int height = 1080;

int main(int argc, char** argv)
{
    tul::SetupArguments(argc, argv);

    StratoGame strato({width, height});

    strato.run();

    return 0;
}