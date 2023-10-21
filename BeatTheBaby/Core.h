
#ifndef __CORE__
#define __CORE__

#include "Jukebox.h"
#include "World.h"
#include "Metronome.h"
#include <SFML/system.hpp>

class Core {
public:
	virtual void startGame() = 0;
	virtual void setMetronome(int bpm) = 0;
	virtual void setJukebox(Jukebox soundSet) = 0;
	virtual void setWorld(World world) = 0;
};

class CoreImpl : public Core {
private:
	World *world;
	Jukebox *jukebox;
	Metronome *metronome;
	sf::Clock clock;

	void gameLoop();
	void waitForNextFrame(long startTime);
public:
	CoreImpl();
	void startGame();
	void setMetronome(int bpm);
	void setJukebox(Jukebox soundSet);
	void setWorld(World world);
};

#endif