#include "Player.h"
#include "ContentPipeline.h"

Player::Player()
	: respawnTimer(0.0f), invincibilityTimer(0.0f)
{
}

void Player::init()
{
	this->setTexture(ContentPipeline::getInstance().getPlayerTexture());
	this->setCollisionCircleRadius(PLAYER_RADIUS);
	this->defineBounds();
}

void Player::defineBounds()
{
	topBound = this->getTexture().getSize().y / 2.0f;
	bottomBound = WORLD_HEIGHT - this->getTexture().getSize().y / 2.0f;
	rightBound = WORLD_WIDTH - this->getTexture().getSize().x / 2.0f;
	leftBound = this->getTexture().getSize().x / 2.0f;
}

void Player::update(const Vector2f& move, const float deltaTime)
{

	if (this->isBoosted())
	{
		this->boostTimer -= deltaTime;
		this->moveSpeed = PLAYER_BOOST_SPEED;
		this->setColor(Color::Yellow);
	}
	else
	{
		this->moveSpeed = PLAYER_SPEED;
		this->setColor(Color::White);
	}

	if (!this->isActive())
	{
		this->respawnTimer -= deltaTime;

		if (this->respawnTimer <= 0.0f) this->respawn();
	}

	if (this->isInvincible())
	{
		this->invincibilityTimer -= deltaTime;

		this->setOpacity(PLAYER_INVINCIBLE_OPACITY);
	}
	else this->setOpacity(PLAYER_DEFAULT_OPACITY);

	this->move(move * moveSpeed * deltaTime);
	this->stayInbound();
}

bool Player::isInvincible() const
{
	return this->invincibilityTimer > 0.0f;
}

bool Player::isBoosted() const
{
	return this->boostTimer > 0.0f;
}

void Player::kill()
{
	if (!this->isActive() || this->isInvincible()) return;

	this->deactivate();
	this->respawnTimer = PLAYER_RESPAWN_COOLDOWN;
}

void Player::boost()
{
	this->boostTimer += BOOST_DURATION;
}

void Player::endBoost()
{
	this->boostTimer = 0.0f;
}

void Player::respawn()
{
	this->activate();
	this->invincibilityTimer = PLAYER_INVINCIBILITY_DURATION;
}

void Player::setOpacity(int opacity)
{
	Color color = this->getColor();
	color.a = opacity;
	this->setColor(color);
}

void Player::stayInbound()
{
	if (this->getPosition().x < leftBound) this->setPosition(leftBound, this->getPosition().y);
	if (this->getPosition().y < topBound) this->setPosition(this->getPosition().x, topBound);

	if (this->getPosition().x > rightBound) this->setPosition(rightBound, this->getPosition().y);
	if (this->getPosition().y > bottomBound) this->setPosition(this->getPosition().x, bottomBound);
}
