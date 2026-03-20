#include "Bullet.h"
#include "ContentPipeline.h"

std::stack<Bullet*> Bullet::bulletStack;
std::stack<Bullet*> Bullet::blastStack;

Bullet::Bullet():
	bulletMove(Vector2f(0.0f, 0.0f)), 
	bulletSound(new Sound(ContentPipeline::getInstance().getShotBuffer())),
	blastSound(new Sound(ContentPipeline::getInstance().getBoostedShotBuffer()))
{
}

Bullet::~Bullet()
{
	if (this->bulletSound != nullptr) delete this->bulletSound;
	if (this->blastSound != nullptr) delete this->blastSound;
}

void Bullet::update(const float deltaTime, const FloatRect& currentViewRectangle)
{
	if (isActive())
	{
		this->move(this->bulletMove * deltaTime);

		if (!this->getGlobalBounds().findIntersection(currentViewRectangle).has_value())
			this->deactivate();
	}
}

void Bullet::shoot(const Vector2f& initialPosition, const Angle& initialAngle)
{
	this->setPosition(initialPosition);

	this->bulletMove.x = BULLET_SPEED * cos(initialAngle.asRadians());
	this->bulletMove.y = BULLET_SPEED * sin(initialAngle.asRadians());

	this->activate();

	if (this->isBlast)
		this->blastSound->play();
	else
		this->bulletSound->play();
}

void Bullet::deactivate()
{
	GameObject::deactivate();

	if (this->isBlast)
		this->addToBlastStack();
	else
		this->addToBulletStack();
}

void Bullet::setType(const unsigned int bulletTypeID)
{
	this->setTexture(ContentPipeline::getInstance().getProjectileTexture(bulletTypeID));
	
	if (bulletTypeID == BLAST_ID)
	{
		this->addToBlastStack();
		this->speed = BLAST_SPEED;
		this->isBlast = true;
	}
	else this->addToBulletStack();
}

void Bullet::addToBulletStack()
{
	bulletStack.push(this);
}

void Bullet::addToBlastStack()
{
	blastStack.push(this);
}

Bullet* Bullet::getAvailableBullet()
{
	if (bulletStack.empty()) return nullptr;

	Bullet* bullet = bulletStack.top();
	bulletStack.pop();
	return bullet;
}

Bullet* Bullet::getAvailableBlast()
{
	if (blastStack.empty()) return nullptr;

	Bullet* blast = blastStack.top();
	blastStack.pop();
	return blast;
}
