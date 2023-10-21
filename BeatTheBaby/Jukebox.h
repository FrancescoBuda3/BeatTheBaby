#ifndef __JUKEBOX__
#define __JUKEBOX__

#include <SFML/Audio.hpp>
#include <iostream>

class Jukebox {
	virtual void playTick() = 0;
	virtual void playBoom() = 0;
	virtual void playClap() = 0;
	virtual void playYes() = 0;
	virtual void playNo() = 0;
	virtual void playGameOver() = 0;
};

class JukeboxImpl : public Jukebox {
private:
	sf::SoundBuffer tickBuffer;
	sf::SoundBuffer boomBuffer;
	sf::SoundBuffer clapBuffer;
	sf::SoundBuffer yesBuffer;
	sf::SoundBuffer noBuffer;
	sf::SoundBuffer gameOverBuffer;
	sf::Sound tick;
	sf::Sound boom;
	sf::Sound clap;
	sf::Sound yes;
	sf::Sound no;
	sf::Sound gameOver;
public:
	JukeboxImpl(std::string tickFile, std::string boomFile,std::string clapFile,
				std::string yesFile, std::string noFile, std::string gameOverFile);
	void playTick();
	void playBoom();
	void playClap();
	void playYes();
	void playNo();
	void playGameOver();
};

#endif 

