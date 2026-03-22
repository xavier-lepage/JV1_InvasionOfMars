#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet();
	~Bullet();
	void update(const float deltaTime, const FloatRect& currentViewRectangle);
	static void updateRecoil(const float deltaTime);
	static void shoot(const Vector2f& initialPosition, const Angle& initialAngle, const bool isBlast);
	void deactivate() override;
	void setType(const unsigned int bulletTypeID);

private:
	static Bullet* getAvailableBullet();
	static Bullet* getAvailableBlast();
	void addToBulletStack();
	void addToBlastStack();

	static std::stack<Bullet*> bulletStack;
	static std::stack<Bullet*> blastStack;
	static float recoil;

	float speed = BULLET_SPEED;
	bool isBlast = false;

	Sound* bulletSound = nullptr;
	Sound* blastSound = nullptr;
	Vector2f bulletMove;
};
