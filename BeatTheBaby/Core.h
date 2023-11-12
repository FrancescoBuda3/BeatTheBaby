
#ifndef __CORE__
#define __CORE__

#include "Jukebox.h"
#include "World.h"
#include "Metronome.h"
#include "Buzz.h"
#include "Beat.h"
#include "View.h"
#include <SFML/system.hpp>


#define INTRO_TIME 5 //in metronome ticks
#define SHOW_TIME 3 //in metronome ticks
#define PLAY_TIME 4 //in metronome ticks
#define DELAY_TIME 1 //in metronome ticks
#define FEEDBACK_TIME 1 //in metronome ticks

enum gameState { MENU, INTRO, SHOW, DELAY, FEEDBACK, PLAY, GAMEOVER };

class Core {
public:
	virtual void startGame(int argc, char* argv[]) = 0;
	virtual void setMetronome(int bpm) = 0;
	virtual void setJukebox(Jukebox *soundSet) = 0;
	virtual void setWorld(World *world) = 0;
	virtual void setBuzz(Buzz *buzz) = 0;
};

class CoreImpl : public Core {
	
public:
	CoreImpl();
	void initGame(int argc, char* argv[]);
	void startGame(int argc, char* argv[]);
	void setMetronome(int bpm);
	void setJukebox(Jukebox *soundSet);
	void setWorld(World *world);
	void setBuzz(Buzz *buzz);
	static void gameLoop(int value);
	static void update();
	static void waitForNextFrame(long startTime);
};

#endif