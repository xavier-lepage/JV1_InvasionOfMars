#pragma once
#include "GameObject.h"

class Alien : public GameObject
{
public:
	Alien();
	void init(const unsigned int alienTextureID);
	static void setPlayer(const GameObject* player);
	void update(float deltaTime);
	Vector2f findSpawnPosition() const;
	void spawn();
	void deactivate() override;
	static Alien* getAvailableAlien();

private:
	void addToAlienStack();

	static std::stack<Alien*> alienStack;
	static const GameObject* player;
};
