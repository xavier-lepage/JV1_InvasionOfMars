#pragma once
#include "GameObject.h"

using std::min;

class Alien : public GameObject
{
public:
	Alien();
	~Alien();
	void init(const unsigned int alienTextureID);
	static void setPlayer(const GameObject* player);
	void update(float deltaTime);
	Vector2f findSpawnPosition() const;
	static void spawnAliens(const float deltaTime);
	bool isSpawning();
	void deactivate() override;

private:
	static Alien* getAvailableAlien();
	void addToAlienStack();

	float spawnAnimationTimer;

	static std::stack<Alien*> alienStack;
	static const GameObject* player;
	static float alienSpawnTimer;

	Sound* alienDeathSound = nullptr;
};
