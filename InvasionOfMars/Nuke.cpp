#include "Nuke.h"
#include "ContentPipeline.h"

std::stack<Nuke*> Nuke::nukeStack;

Nuke::Nuke()
	: nukeTriggerSound(new Sound(ContentPipeline::getInstance().getExplosionSoundBuffer()))
{
	this->addToStack();
}

Nuke::~Nuke()
{
	if (this->nukeTriggerSound != nullptr) delete this->nukeTriggerSound;
}

void Nuke::init()
{
	this->setTexture(ContentPipeline::getInstance().getLogoTexture(NUKE_ID));
}

void Nuke::addToStack()
{
	nukeStack.push(this);
}

Nuke* Nuke::getAvailableNuke()
{
	if (nukeStack.empty()) return nullptr;

	Nuke* nuke = nukeStack.top();
	nukeStack.pop();
	return nuke;
}

void Nuke::collect()
{
	this->despawn();
	this->nukeTriggerSound->play();
}
