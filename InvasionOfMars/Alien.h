#pragma once
#include "GameObject.h"

class Alien : public GameObject
{
public:
	Alien();
	void init(const int alienTextureNumber);
	static void setPlayer(const GameObject* player);
	Vector2f findSpawnPosition() const;
	void spawn();
	static Alien* getAvailableAlien();

private:
	void addToAlienStack();

	static std::stack<Alien*> alienStack;
	static const GameObject* player;
};
