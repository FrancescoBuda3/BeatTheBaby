#include "Beat.h"

BeatImpl::BeatImpl(std::list<float> clapTimings) {
	this->clapTimings = clapTimings;
};

Score BeatImpl::checkClap(float timing) {
	if (this->isFinished()) {
		return Score::NONE;
	}

	float tmp = this->clapTimings.front();

	if (tmp == 0) {
		tmp = timing < 0.5 ? 0 : 1;
	}
		tmp = abs(tmp - timing);

	
	this->clapTimings.pop_front();

	if (tmp <= PERFECT_TOLLERANCE) {
		return Score::PERFECT;
	}
	else if (tmp <= GOOD_TOLLERANCE) {
		return Score::GOOD;
	}
	else {
		return Score::MISS;
	}
};

bool BeatImpl::isFinished() {
	return this->clapTimings.size() == 0;
};

std::list<float> BeatImpl::getClapTimings() {
	return std::list<float>(this->clapTimings);
};

BeatImpl *BeatImpl::generateRandom() {

	std::list<float> ret = std::list<float>();

	std::random_device dev;
	std::mt19937 gen(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 100);
	if (dist(gen) <= TRIPLET_RATE*100){
		// Triplet
		if (dist(gen) <= DENSITY * 100) {
			ret.push_back(0.0f);
		}
		if (dist(gen) <= DENSITY * 100) {
			ret.push_back(1.0f/3);
		}
		if (dist(gen) <= DENSITY * 100) {
			ret.push_back(2.0f/3);
		}
	}
	else {
		// Normal
		if (dist(gen) <= DENSITY * 100) {
			ret.push_back(0.0f);
		}
		if (dist(gen) <= DENSITY * 100) {
			ret.push_back(1.0f/4);
		}
		if (dist(gen) <= DENSITY * 100) {
			ret.push_back(2.0f/4);
		}
		if (dist(gen) <= DENSITY * 100) {
			ret.push_back(3.0f/4);
		}
	}

	return new BeatImpl(ret);
};

