
#ifndef __CORE__
#define __CORE__

#include "Jukebox.h"
#include "World.h"
#include "Metronome.h"
#include "Buzz.h"
#include "Beat.h"
#include <SFML/system.hpp>

enum gameState { MENU, SHOW, PLAY, PAUSE, GAMEOVER };

class Core {
public:
	virtual void startGame() = 0;
	virtual void setMetronome(int bpm) = 0;
	virtual void setJukebox(Jukebox *soundSet) = 0;
	virtual void setWorld(World *world) = 0;
	virtual void setBuzz(Buzz *buzz) = 0;
	virtual void processInput() = 0;
};

class CoreImpl : public Core {
private:
	World *world;
	Jukebox *jukebox;
	Metronome *metronome;
	sf::Clock clock;
	gameState state;
	Buzz *buzz;

	void gameLoop();
	void waitForNextFrame(long startTime);
	void update();

public:
	CoreImpl();
	void startGame();
	void setMetronome(int bpm);
	void setJukebox(Jukebox *soundSet);
	void setWorld(World *world);
	void setBuzz(Buzz *buzz);
	virtual void processInput();
};

#endif