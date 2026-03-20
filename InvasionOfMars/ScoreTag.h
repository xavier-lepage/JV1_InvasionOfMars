#pragma once
#include "GameObject.h"

using std::to_string;

class ScoreTag : public GameObject
{
public:
	ScoreTag();
	~ScoreTag();
	void update(const float deltaTime);
	void deactivate() override;
	void draw(RenderWindow& renderWindow) const override;
	static void spawn(const Vector2f& spawnLocation, const unsigned int score);

private:
	static ScoreTag* getAvailableScoreTag();
	void addToScoreTagStack();

	float despawnTimer = 0.0f;

	static std::stack<ScoreTag*> scoreTagStack;

	Text* scoreText = nullptr;
};

