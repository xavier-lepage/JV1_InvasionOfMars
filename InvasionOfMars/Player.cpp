#include "Player.h"
#include "ContentPipeline.h"

Player::Player()
{
}

void Player::init()
{
	setTexture(ContentPipeline::getInstance().getPlayerTexture());
}

void Player::rotatePlayer(Vector2f mousePosition)
{	
	setRotation(radians(atan2f(mousePosition.y - getPosition().y, mousePosition.x - getPosition().x)));
}
