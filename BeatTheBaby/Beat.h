#ifndef __BEAT__
#define __BEAT__

#include "Common.h"
#include <list>
#include <random>
#include <iostream>

#define GOOD_TOLLERANCE 0.1 
#define PERFECT_TOLLERANCE 0.05 
#define TRIPLET_RATE 17.0f/100 //17% of the beats are triplets
#define DENSITY 1.0f/2 

class Beat {
public:
	virtual Score checkClap(float timing) = 0;
	virtual bool isFinished() = 0;
	virtual std::list<float> getClapTimings() = 0;
};

class BeatImpl : public Beat {
private:
	std::list<float> clapTimings;
public:
	BeatImpl(std::list<float> clapTimings);
	Score checkClap(float timing);
	bool isFinished();
	std::list<float> getClapTimings();
	static BeatImpl* generateRandom();
};

#endif

