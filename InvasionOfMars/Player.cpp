#include "Player.h"
#include "ContentPipeline.h"


Player::Player()
{

}

void Player::init()
{
	setTexture(ContentPipeline::getInstance().getPlayerTexture());
}
