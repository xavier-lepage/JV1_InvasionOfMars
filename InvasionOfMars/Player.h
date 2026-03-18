#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	void init();
	void update(const Vector2f& move, const float deltaTime);

	bool isInvincible() const;
	bool isBoosted() const;
	void kill();
	void boost();
	void endBoost();

private:
	void respawn();
	void setOpacity(int opacity);
	void defineBounds();
	void stayInbound();

	float respawnTimer;
	float invincibilityTimer;
	float boostTimer = 0.0f;

	float moveSpeed = PLAYER_SPEED;

	float topBound;
	float bottomBound;
	float rightBound;
	float leftBound;
};
