#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameObject.h"

using namespace sf;

class Bullet : public GameObject
{
public:
	Bullet();
	~Bullet();
	void update(const float deltaTime, const FloatRect& currentViewRectangle);
	void shoot(const Vector2f& initialPosition, const Angle& initialAngle);
	static Bullet* getAvailableBullet();

private:
	void addToBulletStack();

	static const float BULLET_SPEED;

	static std::stack<Bullet*> bulletStack;

	Sound* bulletSound = nullptr;
	Vector2f bulletMove;
};
