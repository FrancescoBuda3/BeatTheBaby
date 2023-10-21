#include "Jukebox.h"

JukeboxImpl::JukeboxImpl(std::string tickFile, std::string boomFile, std::string clapFile,
						 std::string yesFile, std::string noFile, std::string gameOverFile) {
	
	if (!this->tickBuffer.loadFromFile(tickFile)) {
		std::cout << "Error loading file" << std::endl;
	}
	this->tick = sf::Sound(this->tickBuffer);

	if (!this->boomBuffer.loadFromFile(boomFile)) {
		std::cout << "Error loading file" << std::endl;
	}
	this->boom = sf::Sound(this->boomBuffer);

	if (!this->clapBuffer.loadFromFile(clapFile)) {
		std::cout << "Error loading file" << std::endl;
	}
	this->clap = sf::Sound(this->clapBuffer);

	if (!this->yesBuffer.loadFromFile(yesFile)) {
		std::cout << "Error loading file" << std::endl;
	}
	this->yes = sf::Sound(this->yesBuffer);

	if (!this->noBuffer.loadFromFile(noFile)) {
		std::cout << "Error loading file" << std::endl;
	}
	this->no = sf::Sound(this->noBuffer);

	if (!this->gameOverBuffer.loadFromFile(gameOverFile)) {
		std::cout << "Error loading file" << std::endl;
	}
	this->gameOver = sf::Sound(this->gameOverBuffer);
}

