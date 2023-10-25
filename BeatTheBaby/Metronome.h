#ifndef __METRONOME__
#define __METRONOME__

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class Metronome {
public:
	virtual float getPos() = 0;
	virtual int getBpm() = 0;
	virtual bool checkTick() = 0;
	virtual void start() = 0;
};

class MetronomeImpl : public Metronome {
private:
	int bpm;
	long timeUnit; //in milliseconds
	bool started;
	sf::Clock clock;
public:
	MetronomeImpl(int bpm);
	float getPos();
	int getBpm();
	bool checkTick();
	void start();
};

#endif