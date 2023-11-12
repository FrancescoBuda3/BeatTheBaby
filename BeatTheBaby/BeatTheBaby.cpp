#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "Core.h"

int main(int argc, char* argv[]) {
    
    Core *core = new CoreImpl();
    core->startGame(argc, argv);

    return 0;
}