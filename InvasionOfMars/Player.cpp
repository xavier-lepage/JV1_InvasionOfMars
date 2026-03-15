#include "Player.h"
#include "ContentPipeline.h"
#include "Constants.h"

Player::Player()
	: remainingLives(PLAYER_LIFE_COUNT), respawnTimer(0.0f), invincibilityTimer(0.0f)
{
}

void Player::init()
{
	setTexture(ContentPipeline::getInstance().getPlayerTexture());
}

void Player::update(float deltaTime)
{
	if (!this->isActive())
	{
		this->respawnTimer -= deltaTime;

		if (this->respawnTimer <= 0.0f) this->respawn();
	}
	else if (this->isInvincible())
	{
		this->invincibilityTimer -= deltaTime;

		this->setOpacity(PLAYER_INVINCIBLE_OPACITY);
	}
	else this->setOpacity(PLAYER_DEFAULT_OPACITY);
}

bool Player::isInvincible() const
{
	return this->invincibilityTimer > 0.0f;
}

void Player::kill()
{
	if (!this->isActive() || this->isInvincible()) return;

	remainingLives--;
	this->deactivate();
	this->respawnTimer = PLAYER_RESPAWN_COOLDOWN;
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
