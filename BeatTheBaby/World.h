#ifndef __WORLD__
#define __WORLD__

#include "Common.h"
#include "Beat.h"
#include <list>

class World {
public:
	virtual Score clap(float pos) = 0;
	virtual std::list<float> getTimeline() = 0;
	virtual long getScore() = 0;
	virtual int getLives() = 0;
	virtual void generateNextWave() = 0;
};

class WorldImpl : public World {
private:
	std::list<BeatImpl> currentWave;
	int lives;
	long score;
public:
	WorldImpl();
	Score clap(float pos);
	std::list<float> getTimeline();
	long getScore();
	int getLives();
	void generateNextWave();
	bool isWaveOver();
};

#endif

