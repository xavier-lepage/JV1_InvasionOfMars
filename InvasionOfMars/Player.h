#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	void init();
	void Player::rotatePlayer(Vector2f mousePosition);

private:
};
