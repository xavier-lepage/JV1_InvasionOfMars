#include "Hud.h"
#include "ContentPipeline.h"

Hud::Hud()
{
}

Hud::~Hud()
{
	if (bidon != nullptr) delete bidon;
}

void Hud::hudInit()
{
	bidon = new Text(ContentPipeline::getInstance().getFont(), "Aliens are invading Mars!!!", 50U);
	bidon->setFillColor(Color::Red);
	bidon->setPosition({ 350.0f, 20.0f });
}


void Hud::draw(RenderWindow& renderWindow)
{
	renderWindow.draw(*bidon);
}
