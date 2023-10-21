#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "Core.h"

int main() {
    
    Core *core = new CoreImpl();
    core->startGame();

    return 0;
}