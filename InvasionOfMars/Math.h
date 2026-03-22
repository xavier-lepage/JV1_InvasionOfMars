#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Math
{
public:
	static float generateRandomFloat(int min, int max);
	static bool generateRandomBool();
	static float computeDistance(const Vector2f& origin, const Vector2f& destination);
};

