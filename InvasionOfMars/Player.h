#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	void init();
	void update(float deltaTime);

	bool isInvincible() const;
	void kill();

private:
	void respawn();
	void setOpacity(int opacity);

	float respawnTimer;
	float invincibilityTimer;
};
