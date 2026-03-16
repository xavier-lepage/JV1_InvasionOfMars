#include "Hud.h"
#include "ContentPipeline.h"

Hud::Hud()
{
}

Hud::~Hud()
{
	if (lives != nullptr) delete lives;
	if (score != nullptr) delete score;
	if (pause != nullptr) delete pause;
}

void Hud::hudInit()
{
	lives = new Text(ContentPipeline::getInstance().getFont(), "Lives X ", 24U);
	lives->setFillColor(Color::White);
	lives->setPosition({ 12.0f, 10.0f });

	score = new Text(ContentPipeline::getInstance().getFont(), "Score ", 24U);
	score->setFillColor(Color::White);
	score->setPosition({ 12.0f, 48.0f });

	pause = new Text(ContentPipeline::getInstance().getFont(), "PAUSE", 64U);
	pause->setPosition({ 520.0f, 320.0f });
}

void Hud::update(const unsigned int lives, const unsigned int score, const bool isPaused)
{
	this->lives->setString("Lives X " + to_string(lives));
	this->score->setString("Score " + to_string(score));
	if (isPaused)
		this->pause->setFillColor(Color::White);
	else
		this->pause->setFillColor(Color::Transparent);
}

void Hud::draw(RenderWindow& renderWindow)
{
	renderWindow.draw(*lives);
	renderWindow.draw(*score);
	renderWindow.draw(*pause);
}
