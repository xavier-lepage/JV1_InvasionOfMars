#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Inputs.h"
#include "Player.h"
#include "Alien.h"
#include "Hud.h"
#include "Bullet.h"

using namespace sf;
using std::optional;

class Game
{
public:
	Game();
	int run();

private:
	const int WORLD_CENTER_X = WORLD_WIDTH / 2;
	const int WORLD_CENTER_Y = WORLD_HEIGHT / 2;

	//Puisque la utilise des positions centrales, les limites d'affichage
	//sont les positions centrales des cadres limites.
	const float WORLD_LIMIT_MIN_X = SCREEN_WIDTH / 2.0f;
	const float WORLD_LIMIT_MIN_Y = SCREEN_HEIGHT / 2.0f;
	const float WORLD_LIMIT_MAX_X = WORLD_WIDTH - WORLD_LIMIT_MIN_X;
	const float WORLD_LIMIT_MAX_Y = WORLD_HEIGHT - WORLD_LIMIT_MIN_Y;

	bool init();
	void getInputs();
	void update();
	void draw();
	bool unload();

	void computeDeltaTime();

	void ajustCrossingWorldLimits();
	void keepPlayerInbound();

	void initBullets();
	void updateBullets();
	void drawBullets();

	void initAliens();
	void spawnAliens();
	void updateAliens();
	void drawAliens();

	void fire();
	void handleProjectileCollisions();

	void handlePlayerCollisions();

	RenderWindow renderWindow;
	View mainView;
	View hudView;

    Clock clock;
	float deltaTime;
	float recoilTimer;
	float alienSpawnTimer;


	unsigned int remainingLives = PLAYER_LIFE_COUNT;
	unsigned int score = 0;

	Inputs inputs;

	Hud hud;
	Music music;

	Sprite* field = nullptr;
	Player player;

	Bullet bullets[BULLET_COUNT];

	Alien aliens[ALIEN_COUNT];

	FloatRect currentViewRectangle;

	float topBound;
	float bottomBound;
	float rightBound;
	float leftBound;
};
