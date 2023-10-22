#include "Buzz.h"

BuzzImpl::BuzzImpl() {
	this->Pressed = false;
}

bool BuzzImpl::checkClick() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !this->Pressed) {
		this->Pressed = true;
		return true;
	} 
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && this->Pressed) {
		this->Pressed = false;
	}

	return false;
}