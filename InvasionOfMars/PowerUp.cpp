#include "PowerUp.h"
#include "ContentPipeline.h"

PowerUp::PowerUp()
{
}

void PowerUp::update(const float deltaTime)
{
	if (this->despawnTimer > 0.0f) 
		this->despawnTimer -= deltaTime;
	else if (this->isActive())
		this->despawn();
}

void PowerUp::spawn(const Vector2f& spawnPosition)
{
	this->setPosition(spawnPosition);
	this->activate();
	this->despawnTimer = POWERUP_DURATION;
}

void PowerUp::despawn()
{
	this->deactivate();
	this->addToStack();
}
