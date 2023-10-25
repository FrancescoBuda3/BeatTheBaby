#include "View.h"
#include <iostream>


void ViewImpl::showMenu() {
	std::cout << "showMenu" << std::endl;
	// TODO: Implement
}

void ViewImpl::showGame() {
	std::cout << "showGame" << std::endl;
	// TODO: Implement
}

void ViewImpl::showGameOver() {
	std::cout << "showGameOver" << std::endl;
	// TODO: Implement
}

void ViewImpl::notifyTick() {
	std::cout << "notifyTick" << std::endl;
	// TODO: Implement
}

void ViewImpl::notifyBoom(float pos) {
	std::cout << "notifyBoom" << std::endl;
}

void ViewImpl::notifyClap(Score score) {
	std::cout << "notifyClap" << std::endl;
}

void ViewImpl::notifyYes() {
	std::cout << "notifyYes" << std::endl;
}

void ViewImpl::notifyNo() {
	std::cout << "notifyNo" << std::endl;
}

void ViewImpl::notifyScore(long score) {
	std::cout << "notifyScore" << std::endl;
}

void ViewImpl::notifyLives(int lives) {
	std::cout << "notifyLives" << std::endl;
}

