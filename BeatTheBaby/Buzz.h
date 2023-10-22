#ifndef __BUZZ__
#define __BUZZ__

#include <SFML/Graphics.hpp>
#include <iostream>

class Buzz {
public:
	virtual bool checkClick() = 0;
};

class BuzzImpl : public Buzz {
private:
	bool Pressed;
public:
	BuzzImpl();
	bool checkClick();
};

#endif
