#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

using namespace sf;

struct Inputs
{
	const float DEAD_ZONE = 20.0f;

	Vector2f mousePosition;

	Vector2f move;
	Vector2f aim;

	Angle aimAngle;

	bool rotated;
	bool fire;
	bool pause;
	bool toggleFullscreen;

	Inputs()
	{
		reset();
	}

	void reset()
	{
		mousePosition = { 0.0f, 0.0f };
		move = { 0.0f, 0.0f };
		aim = { 0.0f, 0.0f };

		rotated = false;
		fire = false;
		pause = false;
		toggleFullscreen = false;
	}	

	float manageGamepadAxis(const float axisValue)
	{
		if (abs(axisValue) < DEAD_ZONE) return 0.0f;
		return axisValue / 100.0f;
	}

	void manageDiagonalMovement()
	{
		if (move.x != 0 && move.y != 0)
		{
			move.x *= DIAGONAL_RATIO;
			move.y *= DIAGONAL_RATIO;
		}
	}
};
