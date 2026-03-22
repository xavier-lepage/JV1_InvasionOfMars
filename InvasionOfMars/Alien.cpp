#include "Alien.h"
#include "ContentPipeline.h"

const GameObject* Alien::player = nullptr;

std::stack<Alien*> Alien::alienStack;
float Alien::alienSpawnTimer = 0.0f;

Alien::Alien()
	: alienDeathSound(new Sound(ContentPipeline::getInstance().getAlienDeathBuffer()))
{
	this->addToAlienStack();
}

Alien::~Alien()
{
	if (this->alienDeathSound != nullptr) delete this->alienDeathSound;
}

void Alien::init(const unsigned int alienTextureID)
{
	this->setTexture(ContentPipeline::getInstance().getAlienTexture(alienTextureID));
}

void Alien::setPlayer(const GameObject* player)
{
	Alien::player = player;
}

void Alien::update(float deltaTime)
{
	if (isActive())
	{
		if (!player->isActive() || Math::computeDistance(player->getPosition(), this->getPosition()) > ALIEN_SPEED * deltaTime)
		{
			float angle;

			/*	On détermine l'angle selon si le joueur est en vie. Pour inverser la direction d'un alien
				quand le joueur meurt, on peut simplement inverser le vecteur. */
			if (player->isActive())
				angle = atan2f(player->getPosition().y - getPosition().y, player->getPosition().x - getPosition().x);
			else
				angle = atan2f(getPosition().y - player->getPosition().y, getPosition().x - player->getPosition().x);

			if (this->isSpawning())
			{
				spawnAnimationTimer -= deltaTime;
				this->setRotation(radians(angle + FULL_TURN * spawnAnimationTimer));

				float scale = min(ALIEN_SPAWN_ANIMATION_DURATION - spawnAnimationTimer, NORMAL_SCALE);
				this->setScale({ scale, scale });
			}
			else
			{
				this->setRotation(radians(angle));
				this->move(cos(angle) * ALIEN_SPEED * deltaTime, sin(angle) * ALIEN_SPEED * deltaTime);
			}
		}
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

	//	On itère jusqu'à trouver une position qui se trouve assez loin du joueur.
	do
	{
		position = this->getRandomPosition();
		distance = Math::computeDistance(player->getPosition(), position);
	} while (distance < MIN_ALIEN_DISTANCE);

	return position;
}

void Alien::spawnAliens(const float deltaTime)
{
	if (alienSpawnTimer > 0.0f)
	{
		alienSpawnTimer -= deltaTime;
		return;
	}

	Alien* alien = Alien::getAvailableAlien();

	if (alien == nullptr) return;

	alienSpawnTimer = ALIEN_SPAWN_COOLDOWN;

	alien->setPosition(alien->findSpawnPosition());
	alien->spawnAnimationTimer = ALIEN_SPAWN_ANIMATION_DURATION;
	alien->activate();
}

bool Alien::isSpawning()
{
	return this->spawnAnimationTimer > 0.0f;
}

void Alien::deactivate()
{
	GameObject::deactivate();
	this->addToAlienStack();
	this->alienDeathSound->play();
}
