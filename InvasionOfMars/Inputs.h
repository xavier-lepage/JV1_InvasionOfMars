#pragma once

struct Inputs
{
	const float DEAD_ZONE = 20.0f;

	Inputs()
	{
		reset();
	}

	void reset()
	{
		moveVertical = 0.0f;
		moveHorizontal = 0.0f;
		//Peut-être pratique de sauvegarder le fait
		//que le gamepad soit actif ou non (voir game.getInputs)
		bool isGamepadActive = false;
	}	

	//Portée analogue -100 à 100
	float manageGamepadAxis(const float axisValue)
	{
		if (abs(axisValue) < DEAD_ZONE) return 0.0f;
		return axisValue / 100.0f;
	}

	float moveVertical;
	float moveHorizontal;
	bool isGamepadActive;
};

