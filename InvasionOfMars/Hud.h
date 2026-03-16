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
	void update(unsigned int lives, unsigned int score);
	void draw(RenderWindow& renderWindow);

private:
	Text* lives = nullptr;
	Text* score = nullptr;
};
