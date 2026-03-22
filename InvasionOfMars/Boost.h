#pragma once
#include "PowerUp.h"

class Boost : public PowerUp
{
public:
	Boost();
	~Boost();
	void init() override;
	static Boost* getAvailableBoost();
	void collect() override;

private:
	void addToStack() override;

	static std::stack<Boost*> boostStack;

	Sound* boostTriggerSound = nullptr;
};

