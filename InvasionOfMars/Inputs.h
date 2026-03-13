#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

using namespace sf;

struct Inputs
{
	const float DEAD_ZONE = 20.0f;

	float moveVertical;
	float moveHorizontal;
	bool isGamepadActive;
	Vector2f mousePosition;

	Inputs()
	{
		reset();
	}

	void reset()
	{
		moveVertical = 0.0f;
		moveHorizontal = 0.0f;
		mousePosition = { 0.0f, 0.0f };

		//Peut-être pratique de sauvegarder le fait
		//que le gamepad soit actif ou non (voir game.getInputs)
		isGamepadActive = false;
	}	

	//Portée analogue -100 à 100
	float manageGamepadAxis(const float axisValue)
	{
		if (abs(axisValue) < DEAD_ZONE) return 0.0f;
		return axisValue / 100.0f;
	}

	void manageDiagonalMovement()
	{
		if (moveHorizontal != 0 && moveVertical != 0)
		{
			moveHorizontal *= DIAGONAL_RATIO;
			moveVertical *= DIAGONAL_RATIO;
		}
	}
};
