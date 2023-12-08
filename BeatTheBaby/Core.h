
#ifndef __CORE__
#define __CORE__

#include "Jukebox.h"
#include "World.h"
#include "Metronome.h"
#include "Buzz.h"
#include "Beat.h"
#include "View.h"
#include "Core.h"
#include <SFML/system.hpp>


#define INTRO_TIME 5 //in metronome ticks
#define SHOW_TIME 3 //in metronome ticks
#define PLAY_TIME 4 //in metronome ticks
#define DELAY_TIME 1 //in metronome ticks
#define FEEDBACK_TIME 1 //in metronome ticks

enum gameState { MENU, INTRO, SHOW, DELAY, FEEDBACK, PLAY, GAMEOVER };


void initGame(int argc, char* argv[]);
void startGame(int argc, char* argv[]);
void gameLoop(int value);
void update();
void waitForNextFrame(long startTime);


#endif