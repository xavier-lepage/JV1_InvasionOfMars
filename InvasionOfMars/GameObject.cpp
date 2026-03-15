#include "GameObject.h"

Texture GameObject::emptyTexture;

GameObject::GameObject() : Sprite(emptyTexture)
{
}

GameObject::GameObject(const Texture& texture) : Sprite(texture)
{
	setOrigin({ texture.getSize().x / 2.0f, texture.getSize().y / 2.0f });
	useHorizontalAxisForCircleCollision();
}

void GameObject::setTexture(const Texture& texture)
{
	Sprite::setTexture(texture, true);
	setOrigin({ texture.getSize().x / 2.0f, texture.getSize().y / 2.0f });
	useHorizontalAxisForCircleCollision();
}

void GameObject::activate()
{
	active = true;
}

void GameObject::deactivate()
{
	active = false;
}

bool GameObject::isActive() const
{
	return active;
}

const CollisionCircle& GameObject::getCollisionCircle() const
{
	return collisionCircle;
}

float GameObject::getCollisionCircleRadius() const
{
	return collisionCircle.getRadius();
}

void GameObject::setCollisionCircleRadius(const float radius)
{
	collisionCircle.setRadius(radius);
}

void GameObject::useHorizontalAxisForCircleCollision()
{
	collisionCircle.setRadius(getTexture().getSize().x / 2.0f);
}

void GameObject::useVerticalAxisForCircleCollision()
{
	collisionCircle.setRadius(getTexture().getSize().y / 2.0f);
}

bool GameObject::isCircleColliding(const GameObject& other) const
{
	if (active && other.active)
		return this->getCollisionCircle().checkCollision(other.getCollisionCircle());

	return false;
}

void GameObject::setPosition(const float x, const float y)
{
	setPosition({ x, y });
}

void GameObject::setPosition(const Vector2f& position)
{
	Sprite::setPosition(position);
	collisionCircle.setPosition(position.x, position.y);
}

void GameObject::move(const float offsetX, const float offsetY)
{
	move({ offsetX, offsetY });
}

void GameObject::move(const Vector2f& offset)
{
	if (active)
	{
		backupPosition = getPosition();
		Sprite::move(offset);
		collisionCircle.setPosition(getPosition().x, getPosition().y);
	}
}

void GameObject::moveBack()
{
	setPosition(backupPosition);
}

void GameObject::draw(RenderWindow& renderWindow) const
{
	if (active) renderWindow.draw(*this);
}

Vector2f GameObject::getRandomPosition() const
{
	int collisionCircleRadius = (int)this->getCollisionCircleRadius();
	return
	{
		// Source consultée pour la génération de nombres aléatoires:
		// https://www.geeksforgeeks.org/c/generating-random-number-range-c/
		(float)(collisionCircleRadius + rand() % (WORLD_WIDTH - 2 * collisionCircleRadius + 1)),
		(float)(collisionCircleRadius + rand() % (WORLD_HEIGHT - 2 * collisionCircleRadius + 1))
	};
}

float GameObject::computeDistance(const Vector2f& origin, const Vector2f& destination)
{
	// Documentation consultée pour les méthodes et opérateurs surchargés de Vector2:
	// https://www.sfml-dev.org/documentation/3.0.2/classsf_1_1Vector2.html
	return (destination - origin).length();
}
