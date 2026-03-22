#include "Math.h"

float Math::generateRandomFloat(int min, int max)
{
	// Source consultée pour la génération de nombres aléatoires:
	// https://www.geeksforgeeks.org/c/generating-random-number-range-c/
	return (float)(min + rand() % (max - (2 * min) + 1));
}

bool Math::generateRandomBool()
{
	return (bool)generateRandomFloat(0, 1);
}

float Math::computeDistance(const Vector2f& origin, const Vector2f& destination)
{
	// Documentation consultée pour les méthodes et opérateurs surchargés de Vector2:
	// https://www.sfml-dev.org/documentation/3.0.2/classsf_1_1Vector2.html
	return (destination - origin).length();
}
