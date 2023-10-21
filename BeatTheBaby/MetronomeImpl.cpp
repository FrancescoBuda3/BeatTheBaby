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
	return this->clock.getElapsedTime().asMilliseconds()/this->timeUnit;
}

float MetronomeImpl::getBpm() {
	return this->bpm;
}


