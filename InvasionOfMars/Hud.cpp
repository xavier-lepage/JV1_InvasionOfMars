#include "Hud.h"
#include "ContentPipeline.h"

Hud::Hud()
{
}

Hud::~Hud()
{
	if (lives != nullptr) delete lives;
	if (score != nullptr) delete score;
}

void Hud::hudInit()
{
	lives = new Text(ContentPipeline::getInstance().getFont(), "Lives X ", 24U);
	lives->setFillColor(Color::White);
	lives->setPosition({ 12.0f, 10.0f });

	score = new Text(ContentPipeline::getInstance().getFont(), "Score ", 24U);
	score->setFillColor(Color::White);
	score->setPosition({ 12.0f, 48.0f });
}

void Hud::update(unsigned int lives, unsigned int score)
{
	this->lives->setString("Lives X " + to_string(lives));
	this->score->setString("Score " + to_string(score));
}

void Hud::draw(RenderWindow& renderWindow)
{
	renderWindow.draw(*lives);
	renderWindow.draw(*score);
}
