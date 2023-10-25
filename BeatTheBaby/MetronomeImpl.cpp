#include "Metronome.h"



MetronomeImpl::MetronomeImpl(int bpm) {
	this->bpm = bpm;
	this->timeUnit = std::round((60.0f/bpm) * 1000.0f);
	this->started = false;
}

bool MetronomeImpl::checkTick() {
	if (!this->started) {
		return false;
	}
	if (this->clock.getElapsedTime().asMilliseconds() >= this->timeUnit) {
		this->clock.restart();
		return true;
	}
	return false;
}

float MetronomeImpl::getPos() {
	return static_cast<float>(this->clock.getElapsedTime().asMilliseconds())/this->timeUnit;
}

int MetronomeImpl::getBpm() {
	return this->bpm;
}

void MetronomeImpl::start() {
	this->clock.restart();
	this->started = true;
}


