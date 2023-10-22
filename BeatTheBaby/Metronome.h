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
};

class MetronomeImpl : public Metronome {
private:
	int bpm;
	long timeUnit; //in milliseconds
	sf::Clock clock;
	sf::Sound tick;
	sf::SoundBuffer *tickBuffer;
public:
	MetronomeImpl(int bpm);
	float getPos();
	int getBpm();
	bool checkTick();
	
};

#endif