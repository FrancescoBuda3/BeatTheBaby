
#include "Core.h"
#include "Metronome.h"
#include "Buzz.h"
#include <iostream>
#include <SFML/Graphics.hpp>

#define PERIOD 1000 / 60


CoreImpl::CoreImpl(){
	metronome = new MetronomeImpl(120);
	buzz = new BuzzImpl();
};

void CoreImpl::gameLoop() {
	long previousFrameStartTime = 0;
	this->clock.restart();
	while (true /*provvisorio*/) {
		long currentFrameStartTime = this->clock.getElapsedTime().asMilliseconds(); 
		long elapsed = currentFrameStartTime - previousFrameStartTime;
		this->processInput();
		//this.update(elapsed);
		//this.render();
		this->waitForNextFrame(currentFrameStartTime);
		previousFrameStartTime = currentFrameStartTime;
	}
};

void CoreImpl::update() {
	switch (this->state)
	{
	case MENU:
			break;
	case SHOW:
			break;
	case PLAY:
			break;
	case PAUSE:
			break;
	case GAMEOVER:
			break;
	}
}

void CoreImpl::waitForNextFrame(long startTime) {
	long elapsed = this->clock.getElapsedTime().asMilliseconds() - startTime;
	if (elapsed < PERIOD) {
		sf::sleep(sf::milliseconds(PERIOD - elapsed));
	}	
};

void CoreImpl::startGame() {
	this->gameLoop();
};

void CoreImpl::setMetronome(int bpm) {
	this->metronome = new MetronomeImpl(bpm);
};

void CoreImpl::setJukebox(Jukebox *jukebox) {
	this->jukebox = jukebox;
};

void CoreImpl::setWorld(World *world) {
	this->world = world;
};

void CoreImpl::setBuzz(Buzz* buzz) {
	this->buzz = buzz;
};

void CoreImpl::processInput() {
	if (this->buzz->checkClick()) {
		std::cout << "Buzzed!" << std::endl;
	}
};
