#include "Boost.h"
#include "ContentPipeline.h"

std::stack<Boost*> Boost::boostStack;

Boost::Boost()
	: boostTriggerSound(new Sound(ContentPipeline::getInstance().getTokenSoundBuffer()))
{
	this->addToStack();
}

Boost::~Boost()
{
	if (this->boostTriggerSound != nullptr) delete this->boostTriggerSound;
}

void Boost::init()
{
	this->setTexture(ContentPipeline::getInstance().getLogoTexture(BOOST_ID));
}

void Boost::addToStack()
{
	boostStack.push(this);
}

Boost* Boost::getAvailableBoost()
{
	if (boostStack.empty()) return nullptr;

	Boost* boost = boostStack.top();
	boostStack.pop();
	return boost;
}

void Boost::collect()
{
	this->despawn();
	this->boostTriggerSound->play();
}
