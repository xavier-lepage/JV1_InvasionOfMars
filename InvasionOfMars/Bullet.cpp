#include "Bullet.h"
#include "ContentPipeline.h"

std::stack<Bullet*> Bullet::bulletStack;
std::stack<Bullet*> Bullet::blastStack;
float Bullet::recoil = 0.0f;

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

void Bullet::updateRecoil(const float deltaTime)
{
	if (recoil > 0.0f) recoil -= deltaTime;
}

void Bullet::shoot(const Vector2f& initialPosition, const Angle& initialAngle, const bool isBlast)
{
	if (recoil > 0.0f) return;

	Bullet* projectile;
	if (isBlast) projectile = Bullet::getAvailableBlast();
	else projectile = Bullet::getAvailableBullet();

	if (projectile == nullptr) return;

	projectile->setPosition(initialPosition);
	projectile->bulletMove.x = projectile->speed * cos(initialAngle.asRadians());
	projectile->bulletMove.y = projectile->speed * sin(initialAngle.asRadians());

	projectile->activate();

	if (projectile->isBlast)
	{
		projectile->blastSound->play();
		recoil = BLAST_RECOIL;
	}
	else
	{
		projectile->bulletSound->play();
		recoil = BULLET_RECOIL;
	}
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
