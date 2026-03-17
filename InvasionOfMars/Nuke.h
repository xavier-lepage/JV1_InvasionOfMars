#pragma once
#include "PowerUp.h"

class Nuke : public PowerUp
{
public:
	Nuke();
	void init() override;
	static Nuke* getAvailableNuke();

private:
	void despawn() override;
	void addToStack() override;

	static std::stack<Nuke*> nukeStack;
};

