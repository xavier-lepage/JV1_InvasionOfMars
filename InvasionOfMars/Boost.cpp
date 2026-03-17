#include "Boost.h"
#include "ContentPipeline.h"

std::stack<Boost*> Boost::boostStack;

Boost::Boost()
{
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
