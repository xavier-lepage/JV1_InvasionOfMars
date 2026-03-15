#include "Bullet.h"
#include "ContentPipeline.h"

const float Bullet::BULLET_SPEED = 1200.0f;
std::stack<Bullet*> Bullet::bulletStack;

Bullet::Bullet()
	: bulletMove(Vector2f(0.0f, 0.0f)), bulletSound(new Sound(ContentPipeline::getInstance().getShotBuffer()))
{
	this->addToBulletStack();
}

Bullet::~Bullet()
{
	if (bulletSound != nullptr) delete bulletSound;
}

void Bullet::update(const float deltaTime, const FloatRect& currentViewRectangle)
{
	if (isActive())
	{
		this->move(bulletMove * deltaTime);

		if (!this->getGlobalBounds().findIntersection(currentViewRectangle).has_value())
		{
			this->deactivate();
		}
	}
}

void Bullet::shoot(const Vector2f& initialPosition, const Angle& initialAngle)
{
	this->setPosition(initialPosition);
	this->bulletSound->play();

	this->bulletMove.x = BULLET_SPEED * cos(initialAngle.asRadians());
	this->bulletMove.y = BULLET_SPEED * sin(initialAngle.asRadians());

	this->activate();
}

void Bullet::deactivate()
{
	GameObject::deactivate();
	this->addToBulletStack();
}

void Bullet::addToBulletStack()
{
	bulletStack.push(this);
}

Bullet* Bullet::getAvailableBullet()
{
	if (bulletStack.empty()) return nullptr;

	Bullet* bullet = bulletStack.top();
	bulletStack.pop();
	return bullet;
}
