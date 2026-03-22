#pragma once
#include "PowerUp.h"

class Nuke : public PowerUp
{
public:
	Nuke();
	~Nuke();
	void init() override;
	static Nuke* getAvailableNuke();
	void collect() override;

private:
	void addToStack() override;

	static std::stack<Nuke*> nukeStack;

	Sound* nukeTriggerSound = nullptr;
};

