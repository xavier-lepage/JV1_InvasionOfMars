#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using std::to_string;

class Hud
{
public:
	Hud();
	~Hud();
	void hudInit();
	void update(const unsigned int lives, const unsigned int score, const bool isPaused);
	void draw(RenderWindow& renderWindow);

private:
	Text* lives = nullptr;
	Text* score = nullptr;
	Text* pause = nullptr;
};
