#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Math.h"
#include "Inputs.h"
#include "Player.h"
#include "Alien.h"
#include "Hud.h"
#include "Bullet.h"
#include "Boost.h"
#include "Nuke.h"

using namespace sf;
using std::optional;
using std::min;

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

	void initBullets();
	void updateBullets();
	void drawBullets();

	void initAliens();
	void spawnAliens();
	void updateAliens();
	void drawAliens();
	void onAlienDeath(Alien& alien);

	void rollPowerUp(const Alien& alien);
	void initPowerUps();
	void updatePowerUps();
	void drawPowerUps();

	void increaseScore();

	void fire();
	void handleProjectileCollisions();

	void handlePlayerCollisions();
	void onPlayerDeath();

	void managePause();
	void manageGameOver();

	void onCollectBoost(Boost& boost);
	bool isBoosted();

	RenderWindow renderWindow;
	View mainView;
	View hudView;

    Clock clock;
	float deltaTime;
	float recoilTimer;
	float alienSpawnTimer;

	float comboTimer;
	unsigned int currentCombo = 0;

	unsigned int remainingLives = PLAYER_LIFE_COUNT;
	unsigned int score = 0;

	bool isPaused = false;
	bool isGameOver = false;

	Inputs inputs;

	Hud hud;
	Music music;

	Sprite* field = nullptr;
	Player player;

	Bullet bullets[BULLET_COUNT];
	Bullet blasts[BLAST_COUNT];

	Alien aliens[ALIEN_COUNT];

	Boost boosts[BOOST_COUNT];
	Nuke nukes[NUKE_COUNT];

	FloatRect currentViewRectangle;
};
