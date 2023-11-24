#include "World.h"

WorldImpl::WorldImpl() {
	this->lives = LIVES;
	this->score = 0;
	this->timeLine = {};
};

Score WorldImpl::clap(float pos) {
	std::cout << "prima" << std::endl;
	if (this->timeLine.size() == 0) {
		return Score::NONE;
	}
	else {
		Score ret = this->timeLine.front()->checkClap(pos);
	
		if (ret == Score::NONE) {
			this->timeLine.pop_front();
			return this->clap(pos);
		}
		if (ret == Score::MISS) {
			this->lives == 0? this->lives = 0 : this->lives--;
			std::cout << "Missed! Lives: " << this->lives << std::endl;
		}
		else {
			this->score += ret;
		}

		if (this->timeLine.front()->isFinished()) {
			this->timeLine.pop_front();
		}

		return ret;
	}
};

std::list<float> WorldImpl::getTimeline() {
	std::list<float> ret = {};
	float i = 0;
	for (BeatImpl *beat : this->timeLine) {
		std::list<float> tmp = beat->getClapTimings();
		for (float f : tmp) {
			ret.push_back(f + i);
		}
		i = i+1;
	}
	return ret;
};

long WorldImpl::getScore() {
	return this->score;
};

int WorldImpl::getLives() {
	return this->lives;
};

void WorldImpl::generateNextTimeline() {
	this->timeLine.clear();
	for (int i = 0; i < TIMELINE_SIZE; i++) {
		this->timeLine.push_back(BeatImpl::generateRandom());
	}
};

bool WorldImpl::isTimelineOver() {
	return this->timeLine.size() == 0;
};

void WorldImpl::reset() {
	this->lives = LIVES;
	this->score = 0;
};

