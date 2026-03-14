#include "Player.h"
#include "ContentPipeline.h"
#include "Constants.h"

Player::Player()
{
}

void Player::init()
{
	setTexture(ContentPipeline::getInstance().getPlayerTexture());
}
