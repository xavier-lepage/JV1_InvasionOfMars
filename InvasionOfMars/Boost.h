#pragma once
#include "PowerUp.h"

class Boost : public PowerUp
{
public:
	Boost();
	void init() override;
	static Boost* getAvailableBoost();

private:
	void addToStack() override;

	static std::stack<Boost*> boostStack;
};

