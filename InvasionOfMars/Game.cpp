#include "Game.h"
#include "ContentPipeline.h"

Game::Game()
{	
	mainView = View(FloatRect({ 0.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT }));
	mainView.setCenter({ WORLD_WIDTH / 2.0f , WORLD_HEIGHT / 2.0f });

	hudView = View(FloatRect({ 0.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT }));

	srand((int)time(0));  //Pour générer le seed random
}

int Game::run()
{
	if (!init())return EXIT_FAILURE;

	while (renderWindow.isOpen())
	{
		computeDeltaTime();
		getInputs();
		update();
		draw();
	}

	if (!unload())return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

bool Game::init()
{
	if (!ContentPipeline::getInstance().loadContent()) return false;
	if (!music.openFromFile(std::filesystem::path("Ressources\\Sounds\\Music\\Carpenter_brut_remorse.ogg"))) return false;

	field = new Sprite(ContentPipeline::getInstance().getBackgroundTexture());
	field->setPosition({ 0.0f, 0.0f });
	field->setColor(Color(193, 68, 14, 255)); //Couleur "Rouge sol de Mars"

	player.init();
	player.setPosition((float)WORLD_CENTER_X, (float)WORLD_CENTER_Y);

	initBullets();
	initAliens();
	initPowerUps();

	hud.hudInit();	

	player.activate();
	music.setLooping(true);
	music.play();

	initRenderWindow();
	return true;
}

void Game::initRenderWindow()
{
	/*	Documentation consultée pour la fonctionnalité de plein écran:
	*	https://www.sfml-dev.org/documentation/3.0.2/classsf_1_1RenderWindow.html
	*	https://www.sfml-dev.org/documentation/3.0.2/group__window.html#gga504e2cd8fc6a852463f8d049db1151e5ab13311ab51c4c34757f67f26580018dd
	* 
	*	Documentation consultée pour configurer l'icône sur la fenêtre:
	*	https://www.sfml-dev.org/documentation/3.0.2/classsf_1_1RenderWindow.html
	*	https://www.sfml-dev.org/documentation/3.0.2/classsf_1_1RenderWindow.html#aba4d2434d6c2d058485d8a35b10afb25
	*/
	if (isFullscreen)
		renderWindow.create(VideoMode::getDesktopMode(), "Invasion of Mars", State::Fullscreen);
	else
		renderWindow.create(VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }, 32), "Invasion of Mars");

	renderWindow.setFramerateLimit(144);
	renderWindow.setKeyRepeatEnabled(false);
	renderWindow.setIcon(ContentPipeline::getInstance().getGameIcon());
}

void Game::getInputs()
{
	inputs.reset();

	//On passe l'événement en référence et celui-ci est chargé du dernier événement reçu!
	while (const optional event = renderWindow.pollEvent())
	{
		//x sur la fenêtre
		if (event->is<Event::Closed>())	renderWindow.close();

		if (const Event::JoystickButtonPressed* joystickButtonPressed = event->getIf<Event::JoystickButtonPressed>())
		{
			if (joystickButtonPressed->button == 6) inputs.toggleFullscreen = true;
			if (joystickButtonPressed->button == 7) inputs.pause = true;
		}

		if (Joystick::isConnected(0)) continue;

		if (const Event::KeyPressed* keyPressed = event->getIf<Event::KeyPressed>())
		{
			if (keyPressed->scancode == Keyboard::Scan::F) inputs.toggleFullscreen = true;
			if (keyPressed->scancode == Keyboard::Scan::P) inputs.pause = true;
		}
	}

	if (Joystick::isConnected(0))
	{
		inputs.move.x = inputs.manageGamepadAxis(Joystick::getAxisPosition(0, Joystick::Axis::X));
		inputs.move.y = inputs.manageGamepadAxis(Joystick::getAxisPosition(0, Joystick::Axis::Y));

		inputs.aim.x = inputs.manageGamepadAxis(Joystick::getAxisPosition(0, Joystick::Axis::U));
		inputs.aim.y = inputs.manageGamepadAxis(Joystick::getAxisPosition(0, Joystick::Axis::V));

		float fire = inputs.manageGamepadAxis(Joystick::getAxisPosition(0, Joystick::Axis::Z));
		if (fire < 0.0f) inputs.fire = (bool)fire;

		if (inputs.aim.x != 0.0f || inputs.aim.y != 0.0f)
		{
			inputs.aimAngle = radians(atan2f(inputs.aim.y, inputs.aim.x));
			inputs.rotated = true;
		}
	}
	else
	{
		if (Keyboard::isKeyPressed(Keyboard::Scan::A)) inputs.move.x -= 1.0f;
		if (Keyboard::isKeyPressed(Keyboard::Scan::D)) inputs.move.x += 1.0f;
		if (Keyboard::isKeyPressed(Keyboard::Scan::W)) inputs.move.y -= 1.0f;
		if (Keyboard::isKeyPressed(Keyboard::Scan::S)) inputs.move.y += 1.0f;

		if (Mouse::isButtonPressed(Mouse::Button::Left)) inputs.fire = true;

		inputs.mousePosition = renderWindow.mapPixelToCoords(Mouse::getPosition(renderWindow));

		inputs.aimAngle = radians(atan2f(inputs.mousePosition.y - player.getPosition().y, inputs.mousePosition.x - player.getPosition().x));
		inputs.rotated = true;

		inputs.manageDiagonalMovement();
	}
}

//Vous devrez centrer la vue sur le player: https://www.sfml-dev.org/tutorials/2.6/graphics-view-fr.php
void Game::update()
{
	managePause();
	hud.update(remainingLives, score, isPaused, isGameOver);

	if (inputs.toggleFullscreen)
	{
		isFullscreen = !isFullscreen;
		initRenderWindow();
	}

	if (isPaused || isGameOver) return;

	currentViewRectangle = FloatRect(
		mainView.getCenter() - mainView.getSize() / 2.0f,
		mainView.getSize()
	);

	//On peut déplacer la vue, mais on peut aussi lui la centrer sur une position précise, 
	//comme celle du joueur (avec la méthode setCenter).  Quand votre joueur va se déplacer 
	//vous devrez centrer la vue sur lui.
	if (inputs.rotated) player.setRotation(inputs.aimAngle);
	player.update(inputs.move, deltaTime);

	if (recoilTimer > 0.0f) recoilTimer -= deltaTime;
	if (inputs.fire) fire();
	updateBullets();

	mainView.setCenter(player.getPosition());
	ajustCrossingWorldLimits();

	if (alienSpawnTimer > 0.0f) alienSpawnTimer -= deltaTime;
	spawnAliens();
	updateAliens();
	comboTimer -= deltaTime;

	updatePowerUps();

	handleProjectileCollisions();
	handlePlayerCollisions();
}

void Game::draw()
{
	renderWindow.clear();
	renderWindow.setView(mainView);
	renderWindow.draw(*field);

	drawPowerUps();
	drawBullets();
	drawAliens();

	player.draw(renderWindow);
	
	renderWindow.setView(hudView);
	hud.draw(renderWindow);
	renderWindow.setView(mainView);

	renderWindow.display();
}

void Game::fire()
{
	if (recoilTimer > 0 || !player.isActive()) return;

	if (player.isBoosted())
	{
		Bullet* blast = Bullet::getAvailableBlast();
		if (blast != nullptr)
		{
			recoilTimer = BLAST_RECOIL;

			blast->shoot(
				player.getPosition(),
				player.getRotation()
			);
		}
	}
	else
	{
		Bullet* bullet = Bullet::getAvailableBullet();
		if (bullet != nullptr)
		{
			recoilTimer = BULLET_RECOIL;

			bullet->shoot(
				player.getPosition(),
				player.getRotation()
			);
		}
	}
}

void Game::onPlayerDeath()
{
	remainingLives--;
	manageGameOver();
	player.kill();
}

void Game::handleProjectileCollisions()
{
	for (int i = 0; i < ALIEN_COUNT; i++)
	{
		if (aliens[i].isActive())
		{
			for (int j = 0; j < BULLET_COUNT; j++)
			{
				if (bullets[j].isActive())
				{
					if (aliens[i].isCircleColliding(bullets[j]))
					{
						bullets[j].deactivate();
						onAlienDeath(aliens[i]);
					}
				}
			}

			for (int j = 0; j < BLAST_COUNT; j++)
			{
				if (blasts[j].isActive())
				{
					if (aliens[i].isCircleColliding(blasts[j]))
						onAlienDeath(aliens[i]);
				}
			}
		}
	}
}

void Game::initPowerUps()
{
	for (int i = 0; i < BOOST_COUNT; i++)
		boosts[i].init();

	for (int i = 0; i < NUKE_COUNT; i++)
		nukes[i].init();
}

void Game::updatePowerUps()
{
	for (int i = 0; i < BOOST_COUNT; i++)
		boosts[i].update(deltaTime);

	for (int i = 0; i < NUKE_COUNT; i++)
		nukes[i].update(deltaTime);
}

void Game::drawPowerUps()
{
	for (int i = 0; i < BOOST_COUNT; i++)
		boosts[i].draw(renderWindow);

	for (int i = 0; i < NUKE_COUNT; i++)
		nukes[i].draw(renderWindow);
}

void Game::rollPowerUp(const Alien& alien)
{
	float roll = Math::generateRandomFloat(0, 100) / 100.0f;
	if (roll < POWERUP_CHANCE)
	{
		PowerUp* powerUp = nullptr;

		if (Math::generateRandomBool())
			powerUp = Nuke::getAvailableNuke();
		else
			powerUp = Boost::getAvailableBoost();

		if (powerUp != nullptr) powerUp->spawn(alien.getPosition());
	}
}

void Game::onAlienDeath(Alien& alien)
{
	alien.deactivate();

	rollPowerUp(alien);

	increaseScore();
}

void Game::increaseScore()
{
	int lastScore = score;

	currentCombo++;
	if (comboTimer < 0.0f) currentCombo = 0;
	comboTimer = COMBO_DURATION;

	score += min(SCORE_INCREMENT + (currentCombo * COMBO_INCREMENT), MAX_SCORE_INCREMENT);

	if (lastScore / LIFE_GAIN_SCORE_TRESHOLD < score / LIFE_GAIN_SCORE_TRESHOLD) 
		remainingLives++;
}

void Game::handlePlayerCollisions()
{
	for (int i = 0; i < BOOST_COUNT; i++)
	{
		if (boosts[i].isActive())
			if (player.isCircleColliding(boosts[i])) onCollectBoost(boosts[i]);
	}

	if (!player.isActive() || player.isInvincible()) return;

	for (int i = 0; i < ALIEN_COUNT; i++)
	{
		if (aliens[i].isActive())
		{
			if (player.isCircleColliding(aliens[i]))
			{
				if (player.isBoosted())
				{
					player.endBoost();
					onAlienDeath(aliens[i]);
				}
				else onPlayerDeath();
			}
		}
	}
}

void Game::onCollectBoost(Boost& boost)
{
	boost.despawn();
	player.boost();
}

void Game::initBullets()
{
	for (int i = 0; i < BULLET_COUNT; i++)
		bullets[i].setType(BULLET_ID);

	for (int i = 0; i < BLAST_COUNT; i++)
		blasts[i].setType(BLAST_ID);
}

void Game::updateBullets()
{
	for (int i = 0; i < BULLET_COUNT; i++)
		bullets[i].update(deltaTime, currentViewRectangle);

	for (int i = 0; i < BLAST_COUNT; i++)
		blasts[i].update(deltaTime, currentViewRectangle);
}

void Game::drawBullets()
{
	for (int i = 0; i < BULLET_COUNT; i++)
		bullets[i].draw(renderWindow);

	for (int i = 0; i < BLAST_COUNT; i++)
		blasts[i].draw(renderWindow);
}

bool Game::unload()
{
	if (field != nullptr) delete field;

	return true;
}

void Game::computeDeltaTime()
{
	deltaTime = clock.restart().asSeconds();
}

void Game::ajustCrossingWorldLimits()
{
	if (mainView.getCenter().x < WORLD_LIMIT_MIN_X)
		mainView.setCenter({ WORLD_LIMIT_MIN_X, mainView.getCenter().y });
	else if (mainView.getCenter().x > WORLD_LIMIT_MAX_X)
		mainView.setCenter({ WORLD_LIMIT_MAX_X, mainView.getCenter().y });

	if (mainView.getCenter().y < WORLD_LIMIT_MIN_Y)
		mainView.setCenter({ mainView.getCenter().x, WORLD_LIMIT_MIN_Y });
	else if (mainView.getCenter().y > WORLD_LIMIT_MAX_Y)
		mainView.setCenter({ mainView.getCenter().x, WORLD_LIMIT_MAX_Y });
}

void Game::initAliens()
{
	Alien::setPlayer(&player);

	for (int i = 0; i < ALIEN_COUNT; i++)
		aliens[i].init(rand() % ContentPipeline::ALIEN_TEXTURE_NUMBER);
}

void Game::spawnAliens()
{
	if (alienSpawnTimer > 0) return;

	Alien* alien = Alien::getAvailableAlien();
	if (alien != nullptr)
	{
		alienSpawnTimer = ALIEN_SPAWN_COOLDOWN;

		alien->spawn();
	}
}

void Game::updateAliens()
{
	for (int i = 0; i < ALIEN_COUNT; i++)
		aliens[i].update(deltaTime);
}

void Game::drawAliens()
{
	for (int i = 0; i < ALIEN_COUNT; i++)
		aliens[i].draw(renderWindow);
}

void Game::managePause()
{
	if (inputs.pause)
	{
		isPaused = !isPaused;

		if (isPaused) 
			music.pause();
		else
			music.play();
	}
}

void Game::manageGameOver()
{
	if (remainingLives == 0) {
		isGameOver = true;
		music.stop();
	}
}
