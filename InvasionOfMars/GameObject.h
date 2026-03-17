#pragma once
#include <SFML/Graphics.hpp>
#include "CollisionCircle.h"
#include "Constants.h"
#include "Math.h"
#include <stack>

using namespace sf;

class GameObject : public Sprite
{
public:
	GameObject();
	GameObject(const Texture& texture);
	void setTexture(const Texture& texture);

	void activate();
	virtual void deactivate();
	bool isActive() const;

	void setPosition(const float x, const float y);
	void setPosition(const Vector2f& position);

	const CollisionCircle& getCollisionCircle() const;
	float getCollisionCircleRadius() const;
	void setCollisionCircleRadius(const float radius);
	void useHorizontalAxisForCircleCollision();
	void useVerticalAxisForCircleCollision();
	bool isCircleColliding(const GameObject& other) const;
	


	void move(const float offsetX, const float offsetY);
	void move(const Vector2f& offset);
	void moveBack();

	Vector2f getRandomPosition() const;

	virtual void draw(RenderWindow& renderWindow) const;

private:
	bool active = false;
	CollisionCircle collisionCircle;
	Vector2f backupPosition;

	static Texture emptyTexture;
};
