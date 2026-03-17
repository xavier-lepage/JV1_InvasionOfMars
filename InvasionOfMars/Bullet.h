#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet();
	~Bullet();
	void update(const float deltaTime, const FloatRect& currentViewRectangle);
	void shoot(const Vector2f& initialPosition, const Angle& initialAngle);
	void deactivate() override;
	static Bullet* getAvailableBullet();

private:
	void addToBulletStack();

	static std::stack<Bullet*> bulletStack;

	Sound* bulletSound = nullptr;
	Vector2f bulletMove;
};
