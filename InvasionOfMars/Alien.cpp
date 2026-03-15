#include "Alien.h"
#include "ContentPipeline.h"

const GameObject* Alien::player = nullptr;

std::stack<Alien*> Alien::alienStack;

Alien::Alien()
{
	this->addToAlienStack();
}

void Alien::init(const int alienTextureNumber)
{
	this->setTexture(ContentPipeline::getInstance().getAlienTexture(alienTextureNumber));
}

void Alien::setPlayer(const GameObject* player)
{
	Alien::player = player;
}

void Alien::update(float deltaTime)
{
	if (isActive())
	{
		float angle = atan2f(player->getPosition().y - getPosition().y, player->getPosition().x - getPosition().x);

		this->setRotation(radians(angle));
		this->move(cos(angle) * ALIEN_SPEED * deltaTime, sin(angle) * ALIEN_SPEED * deltaTime);
	}
}

void Alien::addToAlienStack()
{
	alienStack.push(this);
}

Alien* Alien::getAvailableAlien()
{
	if (alienStack.empty()) return nullptr;

	Alien* alien = alienStack.top();
	alienStack.pop();
	return alien;
}

Vector2f Alien::findSpawnPosition() const
{
	Vector2f position;
	float distance;

	do
	{
		position = getRandomPosition();
		distance = computeDistance(player->getPosition(), position);
	} while (distance < MIN_ALIEN_DISTANCE);

	return position;
}

void Alien::spawn()
{	
	this->setPosition(this->findSpawnPosition());

	this->activate();
}
