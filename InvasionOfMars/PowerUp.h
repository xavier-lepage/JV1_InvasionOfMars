#pragma once
#include "GameObject.h"

class PowerUp : public GameObject
{
public:
	PowerUp();
	virtual void init() = 0;
	void update(const float deltaTime);
	void spawn(const Vector2f& spawnPosition);

private:
	void despawn();
	virtual void addToStack() = 0;

	float despawnTimer;
};

