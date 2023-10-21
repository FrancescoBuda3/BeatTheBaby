#include "Metronome.h"



MetronomeImpl::MetronomeImpl(float bpm) {
	this->bpm = bpm;
	this->timeUnit = (60/bpm) * 1000;
	this->clock.restart();
	this->tickBuffer = new sf::SoundBuffer();
	if (!this->tickBuffer->loadFromFile("./Audio/tick.wav")) {
		std::cout << "Error loading tick.wav" << std::endl;
	}
	this->tick = sf::Sound(*this->tickBuffer);
}

bool MetronomeImpl::checkTick() {
	if (this->clock.getElapsedTime().asMilliseconds() >= this->timeUnit) {
		this->clock.restart();
		this->tick.play();
		return true;
	}
	return false;
}

float MetronomeImpl::getPos() {
	// TODO
	return 0;
}

float MetronomeImpl::getBpm() {
	// TODO 
	return 0;
}


