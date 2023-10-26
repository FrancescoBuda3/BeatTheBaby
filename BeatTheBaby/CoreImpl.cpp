
#include "Core.h"


#include <iostream>
#include <SFML/Graphics.hpp>

#define PERIOD 1000 / 60


CoreImpl::CoreImpl(){
	metronome = new MetronomeImpl(100);
	buzz = new BuzzImpl();
	jukebox = new JukeboxImpl("./Audio/tick.wav", "./Audio/boom.wav", "./Audio/clap.wav", "./Audio/yes.wav", "./Audio/no.wav", "./Audio/gameover.wav");
	world = new WorldImpl();
	view = new ViewImpl();
	state = MENU;
	tickCount = 0;
	missed = false;
	timeline = {};
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
		//std::cout << "tick" << std::endl;
	}
	switch (this->state)
	{
	case(MENU):
		if (this->buzz->checkClick()) {
			//std::cout << "intro" << std::endl;
			this->state = INTRO;
			this->metronome->start();
			this->view->showGame();
			this->tickCount = 0;
		}
		break;
	case(INTRO):
		if(this->tickCount == INTRO_TIME) {
			//std::cout << "show" << std::endl;
			this->state = SHOW;
			this->tickCount = 0;
			this->world->reset();
			this->timeline = new std::list<float>(this->world->getTimeline());
		}
		break;
	case(SHOW):
		if (this->tickCount == SHOW_TIME) {
			//std::cout << "yes" << std::endl;
			this->view->notifyYes();
			this->jukebox->playYes();
			this->state = DELAY;
			this->tickCount = 0;
		}
		else if (timeline->size() != 0 && this->metronome->getPos() >= timeline->front() - this->tickCount) {
			//std::cout << "boom" << std::endl;
			this->jukebox->playBoom();
			this->view->notifyBoom(this->timeline->front());
			this->timeline->pop_front();
		}
		break;
	case (DELAY):
		if (this->metronome->getPos() > 0.5) {
			//std::cout << "play" << std::endl;
			this->state = PLAY;
			this->tickCount = 0;
			delete timeline;
			missed = false;
		}
		break;
	case(PLAY):
		if (tickCount == PLAY_TIME) {
			this->world->generateNextTimeline();
			//std::cout << "feedback" << std::endl;
			this->state = FEEDBACK;
			this->tickCount = 0;
			if (this->missed) {
				this->view->notifyNo();
				this->jukebox->playNo();
			}
			else {
				this->view->notifyYes();
				this->jukebox->playYes();
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
		if (this->tickCount == FEEDBACK_TIME) {
			this->tickCount = 0;
			this->timeline = new std::list<float>(this->world->getTimeline());
			this->view->notifyScore(this->world->getScore());
			this->view->notifyLives(this->world->getLives());
			std::cout << "score: " << this->world->getScore() << std::endl;
			if (this->world->getLives() == 0) {
				std::cout << "gameover" << std::endl;
				this->state = GAMEOVER;
				this->view->showGameOver();
			}
			else {
				//std::cout << "show" << std::endl;
				this->state = SHOW;
			}
		}
		break;

	case(GAMEOVER):
		if (this->buzz->checkClick()) {
			//std::cout << "menu" << std::endl;
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



