
#include "Core.h"


#include <iostream>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>


#define PERIOD 1000 / 60

long previousFrameStartTime = 0;
sf::Clock clock_c;
World* world_c;
Jukebox* jukebox_c;
Metronome* metronome_c;
gameState state_c;
Buzz* buzz_c;
ViewImpl* view_c;
std::list<float>* timeline_c;
int tickCount_c;
bool missed_c;

CoreImpl::CoreImpl(){
	metronome_c = new MetronomeImpl(80);
	buzz_c = new BuzzImpl();
	jukebox_c = new JukeboxImpl("./Audio/tick.wav", "./Audio/boom.wav", "./Audio/clap.wav", "./Audio/yes.wav", "./Audio/no.wav", "./Audio/gameover.wav");
	world_c = new WorldImpl();
	view_c = new ViewImpl();
	state_c = MENU;
	tickCount_c = 0;
	missed_c = false;
	timeline_c = {};
	previousFrameStartTime = 0;
};

void CoreImpl::gameLoop(int value) {
	/*long previousFrameStartTime = 0;
	this->initGame();
	while (true) {
		long currentFrameStartTime = this->clock.getElapsedTime().asMilliseconds(); 
		long elapsed = currentFrameStartTime - previousFrameStartTime;
		this->update();
		//this.render();
		this->waitForNextFrame(currentFrameStartTime);
		previousFrameStartTime = currentFrameStartTime;
	}*/
	long currentFrameStartTime = clock_c.getElapsedTime().asMilliseconds();
	long elapsed = currentFrameStartTime - previousFrameStartTime;
	update();
	glutPostRedisplay();
	
	previousFrameStartTime = currentFrameStartTime;
	waitForNextFrame(currentFrameStartTime);

}

void CoreImpl::update() {
	if (metronome_c->checkTick()) {
		jukebox_c->playTick();
		view_c->notifyTick();
		tickCount_c++;
		//std::cout << "tick" << std::endl;
	}
	switch (state_c)
	{
	case(MENU):
		if (buzz_c->checkClick()) {
			//std::cout << "intro" << std::endl;
			state_c = INTRO;
			metronome_c->start();
			view_c->showGame();
			tickCount_c = 0;
		}
		break;
	case(INTRO):
		if(tickCount_c == INTRO_TIME) {
			//std::cout << "show" << std::endl;
			state_c = SHOW;
			tickCount_c = 0;
			world_c->reset();
			timeline_c = new std::list<float>(world_c->getTimeline());
		}
		break;
	case(SHOW):
		if (tickCount_c == SHOW_TIME) {
			//std::cout << "yes" << std::endl;
			view_c->notifyYes();
			jukebox_c->playYes();
			state_c = DELAY;
			tickCount_c = 0;
		}
		else if (timeline_c->size() != 0 && metronome_c->getPos() >= timeline_c->front() - tickCount_c) {
			//std::cout << "boom" << std::endl;
			jukebox_c->playBoom();
			view_c->notifyBoom(timeline_c->front());
			timeline_c->pop_front();
		}
		break;
	case (DELAY):
		if (metronome_c->getPos() > 0.5) {
			//std::cout << "play" << std::endl;
			state_c = PLAY;
			tickCount_c = 0;
			delete timeline_c;
			missed_c = false;
		}
		break;
	case(PLAY):
		if (tickCount_c == PLAY_TIME) {
			world_c->generateNextTimeline();
			//std::cout << "feedback" << std::endl;
			state_c = FEEDBACK;
			tickCount_c = 0;
			if (missed_c) {
				view_c->notifyNo();
				jukebox_c->playNo();
			}
			else {
				view_c->notifyYes();
				jukebox_c->playYes();
			}
		}
		else if (buzz_c->checkClick()) {
			jukebox_c->playClap();
			Score score = world_c->clap(metronome_c->getPos());
			view_c->notifyClap(score, metronome_c->getPos() + tickCount_c -1);
			if (score == MISS) {
				missed_c = true;
			}	
		}
		break;

	case(FEEDBACK):
		if (tickCount_c == FEEDBACK_TIME) {
			std::cout << "\n" << std::endl;
			tickCount_c = 0;
			timeline_c = new std::list<float>(world_c->getTimeline());
			view_c->notifyScore(world_c->getScore());
			view_c->notifyLives(world_c->getLives());
			std::cout << "score: " << world_c->getScore() << std::endl;
			if (world_c->getLives() == 0) {
				std::cout << "gameover" << std::endl;
				state_c = GAMEOVER;
				view_c->showGameOver();
			}
			else {
				//std::cout << "show" << std::endl;
				state_c = SHOW;
			}
		}
		break;

	case(GAMEOVER):
		if (buzz_c->checkClick()) {
			//std::cout << "menu" << std::endl;
			state_c = MENU;
			view_c->showMenu();
		}
		break;
	}
	
}

void CoreImpl::waitForNextFrame(long startTime)
{
	long elapsed = clock_c.getElapsedTime().asMilliseconds() - startTime;
	if (elapsed < PERIOD) {
		//sf::sleep(sf::milliseconds(PERIOD - elapsed));
		glutTimerFunc(PERIOD - elapsed, &CoreImpl::gameLoop, 0);
	}
	else {
		glutTimerFunc(0, &CoreImpl::gameLoop, 0);
	}
}


void CoreImpl::initGame(int argc, char* argv[]) {
	//provv
	glutInit(&argc, argv);
	
	

	//end provv
	view_c->init();
	clock_c.restart();
	view_c->showMenu();
	state_c = MENU;
	world_c->generateNextTimeline();
}

void CoreImpl::startGame(int argc, char* argv[]) {
	//this->gameLoop(0);
	cout << "start" << endl;
	initGame(argc, argv);
	glutTimerFunc(0, &CoreImpl::gameLoop, 0);
	glutMainLoop();
};

void CoreImpl::setMetronome(int bpm) {
	metronome_c = new MetronomeImpl(bpm);
};

void CoreImpl::setJukebox(Jukebox *jukebox) {
	jukebox_c = jukebox;
};

void CoreImpl::setWorld(World *world) {
	world_c = world;
};

void CoreImpl::setBuzz(Buzz* buzz) {
	buzz_c = buzz;
};






