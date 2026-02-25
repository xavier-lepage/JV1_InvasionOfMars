#include "Alien.h"
#include "ContentPipeline.h"

Alien::Alien()
{
}

void Alien::init(const int alienTextureNumber)
{
	setTexture(ContentPipeline::getInstance().getAlienTexture(alienTextureNumber));
}
