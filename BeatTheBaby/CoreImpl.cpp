
#include "Core.h"


#include <iostream>
#include <SFML/Graphics.hpp>

#define PERIOD 1000 / 60


CoreImpl::CoreImpl(){
	metronome = new MetronomeImpl(120);
	buzz = new BuzzImpl();
	jukebox = new JukeboxImpl("./Audio/tick.wav", "./Audio/boom.wav", "./Audio/clap.wav", "./Audio/yes.wav", "./Audio/no.wav", "./Audio/gameover.wav");
	world = new WorldImpl();
	view = new ViewImpl();
	state = MENU;
};

void CoreImpl::gameLoop() {
	long previousFrameStartTime = 0;
	this->initGame();
	//variabili provvisorie
	/*sf::SoundBuffer tickBuffer;
	if (!tickBuffer.loadFromFile("./Audio/tick.wav")) {
		std::cout << "Error loading tick.wav" << std::endl;
	}
	sf::Sound tickSound;
    tickSound.setBuffer(tickBuffer);
	Beat *beat = BeatImpl::generateRandom();
	std::list<float> timeline = beat->getClapTimings();*/
	while (true /*provvisorio*/) {
		long currentFrameStartTime = this->clock.getElapsedTime().asMilliseconds(); 
		long elapsed = currentFrameStartTime - previousFrameStartTime;
		this->update();
		//this.render();
		
		// suona a ritmo
		/*if (timeline.size() == 0 && this->metronome->checkTick()) {
			beat = BeatImpl::generateRandom();
			timeline = beat->getClapTimings();
		}
		if (timeline.size() != 0 && this->metronome->getPos() >= timeline.front()) {
			timeline.pop_front();
			tickSound.play();
		}*/
		this->waitForNextFrame(currentFrameStartTime);
		previousFrameStartTime = currentFrameStartTime;
	}
};

void CoreImpl::update() {
	if (this->metronome->checkTick()) {
		this->jukebox->playTick();
		this->view->notifyTick();
		this->tickCount++;
	}
	switch (this->state)
	{
	case(MENU):
		//std::cout << "menu" << std::endl;
		if (this->buzz->checkClick()) {
			this->state = INTRO;
			this->metronome->start();
			this->view->showGame();
		}
		break;
	case(INTRO):
		if(this->tickCount = INTRO_TIME) {
			this->state = SHOW;
			this->tickCount = 0;
			*this->timeline = this->world->getTimeline();
		}
		break;
	case(SHOW):
		if (this->tickCount = FEEDBACK_TIME) {
			this->view->notifyYes();
		}
		if (this->tickCount = SHOW_TIME) {
			this->state = PLAY;
			this->tickCount = 0;
			this->view->notifyYes();
			delete timeline;
			missed = false;
		}
		else if (timeline->size() != 0 && this->metronome->getPos() >= timeline->front()) {
			this->jukebox->playBoom();
			this->view->notifyBoom(this->timeline->front());
			this->timeline->pop_front();
		}
		break;
	case(PLAY):
		if (tickCount = PLAY_TIME) {
			this->state = FEEDBACK;
			this->tickCount = 0;
			if (this->missed) {
				this->view->notifyNo();
			}
			else {
				this->view->notifyYes();
			}
		}
		else if (this->buzz->checkClick()) {
			this->jukebox->playClap();
			Score score = this->world->clap(this->metronome->getPos());
			this->view->notifyClap(score);
			if (score == MISS) {
				this->missed = true;
			}	
		}
		break;

	case(FEEDBACK):
		if (this->tickCount = FEEDBACK_END_TIME) {
			this->tickCount = 0;
			this->view->notifyScore(this->world->getScore());
			this->view->notifyLives(this->world->getLives());
			if (this->world->getLives() == 0) {
				this->state = GAMEOVER;
				this->view->showGameOver();
			}
			else {
				this->state = SHOW;
			}
		}
		break;

	case(GAMEOVER):
		if (this->buzz->checkClick()) {
			this->state = MENU;
			this->view->showMenu();
		}
		break;
	}
	
}

void CoreImpl::waitForNextFrame(long startTime) {
	long elapsed = this->clock.getElapsedTime().asMilliseconds() - startTime;
	if (elapsed < PERIOD) {
		sf::sleep(sf::milliseconds(PERIOD - elapsed));
	}	
};

void CoreImpl::initGame() {
	this->clock.restart();
	this->view->showMenu();
	this->state = MENU;
	this->world->generateNextTimeline();
};

void CoreImpl::startGame() {
	this->gameLoop();
};

void CoreImpl::setMetronome(int bpm) {
	this->metronome = new MetronomeImpl(bpm);
};

void CoreImpl::setJukebox(Jukebox *jukebox) {
	this->jukebox = jukebox;
};

void CoreImpl::setWorld(World *world) {
	this->world = world;
};

void CoreImpl::setBuzz(Buzz* buzz) {
	this->buzz = buzz;
};



