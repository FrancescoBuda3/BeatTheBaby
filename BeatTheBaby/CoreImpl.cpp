
#include "Core.h"
#include <iostream>

#define PERIOD 1000 / 60




void CoreImpl::gameLoop() {
	long previousFrameStartTime = 0;
	this->clock.restart();
	while (true /*provvisorio*/) {
		long currentFrameStartTime = this->clock.getElapsedTime().asMilliseconds(); 
		long elapsed = currentFrameStartTime - previousFrameStartTime;
		//this.processInput();
		//this.update(elapsed);
		//this.render();
		std::cout << "Elapsed: " << elapsed << std::endl;
		this->waitForNextFrame(currentFrameStartTime);
		previousFrameStartTime = currentFrameStartTime;
	}
};

void CoreImpl::waitForNextFrame(long startTime) {
	long elapsed = this->clock.getElapsedTime().asMilliseconds() - startTime;
	if (elapsed < PERIOD) {
		sf::sleep(sf::milliseconds(PERIOD - elapsed));
	}	
};


CoreImpl::CoreImpl() {
	//world = new WorldImpl();
	//jukebox = new JukeboxImpl();
	//metronome = new MetronomeImpl();
};

void CoreImpl::startGame() {
	this->gameLoop();
};

void CoreImpl::setMetronome(int bpm) {
	// TODO
};

void CoreImpl::setJukebox(Jukebox jukebox) {
	// TODO
};

void CoreImpl::setWorld(World world) {
	// TODO
};
