
#include "Core.h"
#include "View.h"


#include <iostream>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>


#define PERIOD 1000 / 60

long previousFrameStartTime;
sf::Clock clock_c;
World* world_c;
Jukebox* jukebox_c;
Metronome* metronome_c;
gameState state_c;
Buzz* buzz_c;
std::list<float>* timeline_c;
int tickCount_c;
bool missed_c;


void gameLoop(int value) {
	long currentFrameStartTime = clock_c.getElapsedTime().asMilliseconds();
	long elapsed = currentFrameStartTime - previousFrameStartTime;
	update();
	updateHead(elapsed);
	updateBalls(elapsed);
	glutPostRedisplay();
	
	previousFrameStartTime = currentFrameStartTime;
	waitForNextFrame(currentFrameStartTime);

}

void update() {
	if (metronome_c->checkTick()) {
		jukebox_c->playTick();
		notifyTick();
		tickCount_c++;
	}
	switch (state_c)
	{
	case(MENU):
		if (buzz_c->checkClick()) {
			state_c = INTRO;
			metronome_c->start();
			showGame();
			tickCount_c = 0;
		}
		break;
	case(INTRO):
		if(tickCount_c == INTRO_TIME) {
			state_c = SHOW;
			tickCount_c = 0;
			world_c->reset();
			timeline_c = new std::list<float>(world_c->getTimeline());
		}
		break;
	case(SHOW):
		if (tickCount_c == SHOW_TIME) {
			notifyYes();
			jukebox_c->playYes();
			state_c = DELAY;
			tickCount_c = 0;
		}
		else if (timeline_c->size() != 0 && metronome_c->getPos() >= timeline_c->front() - tickCount_c) {
			jukebox_c->playBoom();
			notifyBoom(timeline_c->front());
			timeline_c->pop_front();
		}
		break;
	case (DELAY):
		if (metronome_c->getPos() > 0.5) {
			state_c = PLAY;
			tickCount_c = 0;
			delete timeline_c;
			missed_c = false;
		}
		break;
	case(PLAY):
		if (tickCount_c == PLAY_TIME) {
			
			if (missed_c || !world_c->isTimelineOver()) {
				notifyNo();
				jukebox_c->playNo();
			}
			else {
				notifyYes();
				jukebox_c->playYes();
			}
			world_c->generateNextTimeline();
			state_c = FEEDBACK;
			tickCount_c = 0;
			notifyTime((1.0 / metronome_c->getBpm()) * 60 * 1000);
		}
		else if (buzz_c->checkClick()) {
			jukebox_c->playClap();
			Score score = world_c->clap(metronome_c->getPos());
			notifyClap(score, metronome_c->getPos() + tickCount_c -1);

			if (score == MISS) {
				missed_c = true;
			}	
		}
		break;

	case(FEEDBACK):
		if (tickCount_c == FEEDBACK_TIME) {
			tickCount_c = 0;
			timeline_c = new std::list<float>(world_c->getTimeline());
			notifyScore(world_c->getScore());
			notifyLives(world_c->getLives());
			std::cout << "score: " << world_c->getScore() << std::endl;
			if (world_c->getLives() == 0) {
				state_c = GAMEOVER;
				showGameOver();
			}
			else {
				state_c = SHOW;
			}
		}
		break;

	case(GAMEOVER):
		if (buzz_c->checkClick()) {
			state_c = MENU;
			showMenu();
		}
		break;
	}
	
}

void waitForNextFrame(long startTime)
{
	long elapsed = clock_c.getElapsedTime().asMilliseconds() - startTime;
	if (elapsed < PERIOD) {
		glutTimerFunc(PERIOD - elapsed, &gameLoop, 0);
	}
	else {
		glutTimerFunc(0, &gameLoop, 0);
	}
}


void initGame(int argc, char* argv[]) {

	previousFrameStartTime = 0;
	world_c = new WorldImpl();
	jukebox_c = new JukeboxImpl("./Audio/tick.wav", "./Audio/boom.wav", "./Audio/clap.wav", "./Audio/yes.wav", "./Audio/no.wav", "./Audio/gameover.wav");
	metronome_c = new MetronomeImpl(80);
	state_c = MENU;
	buzz_c = new BuzzImpl();
	timeline_c = {};
	tickCount_c = 0;
	missed_c = false;
	
	glutInit(&argc, argv);
	
	


	init();
	clock_c.restart();
	showMenu();
	notifyTime((1.0 / metronome_c->getBpm()) * 60 * 1000);
	state_c = MENU;
	world_c->generateNextTimeline();
}

void startGame(int argc, char* argv[]) {
	initGame(argc, argv);
	glutTimerFunc(0, &gameLoop, 0);
	glutMainLoop();
};








