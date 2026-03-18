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
	void setType(const unsigned int bulletTypeID);
	static Bullet* getAvailableBullet();
	static Bullet* getAvailableBlast();

private:
	void addToBulletStack();
	void addToBlastStack();

	static std::stack<Bullet*> bulletStack;
	static std::stack<Bullet*> blastStack;

	float speed = BULLET_SPEED;
	bool isBlast = false;

	Sound* bulletSound = nullptr;
	Vector2f bulletMove;
};
