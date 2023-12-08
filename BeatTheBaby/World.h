#ifndef __WORLD__
#define __WORLD__

#include "Common.h"
#include "Beat.h"
#include <list>

#define LIVES 5
#define TIMELINE_SIZE 3

class World {
public:
	virtual Score clap(float pos) = 0;
	virtual std::list<float> getTimeline() = 0;
	virtual long getScore() = 0;
	virtual int getLives() = 0;
	virtual void generateNextTimeline() = 0;
	virtual bool isTimelineOver() = 0; 
	virtual void reset() = 0;
};

class WorldImpl : public World {
private:
	std::list<BeatImpl*> timeLine;
	int lives;
	long score;
public:
	WorldImpl();
	Score clap(float pos);
	std::list<float> getTimeline();
	long getScore();
	int getLives();
	void generateNextTimeline();
	bool isTimelineOver();
	void reset();
};

#endif

