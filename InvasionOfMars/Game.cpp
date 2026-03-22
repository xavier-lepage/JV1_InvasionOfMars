#include "Game.h"
#include "ContentPipeline.h"

Game::Game()
{	
	mainView = View(FloatRect({ 0.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT }));
	mainView.setCenter({ WORLD_WIDTH / 2.0f , WORLD_HEIGHT / 2.0f });

	hudView = View(FloatRect({ 0.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT }));

	srand((int)time(0));  // Pour générer le seed random
}

int Game::run()
{
	if (!init()) return EXIT_FAILURE;

	while (renderWindow.isOpen())
	{
		computeDeltaTime();
		getInputs();
		update();
		draw();
	}

	if (!unload()) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

bool Game::init()
{
	if (!ContentPipeline::getInstance().loadContent()) return false;
	if (!loadMusic()) return false;

	field = new Sprite(ContentPipeline::getInstance().getBackgroundTexture());
	field->setPosition({ 0.0f, 0.0f });
	field->setColor(Color(193, 68, 14, 255)); // Couleur "Rouge sol de Mars"

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

	while (const optional event = renderWindow.pollEvent())
	{
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

void Game::update()
{
	if (inputs.toggleFullscreen)
	{
		isFullscreen = !isFullscreen;
		initRenderWindow();
	}

	hud.update(remainingLives, score, isPaused, isGameOver);
	if (isGameOver) return;
	managePause();
	if (isPaused) return;

	manageViewUpdates();
	managePlayerUpdates();
	manageBulletUpdates();

	updateScoreTags();
	Alien::spawnAliens(deltaTime);
	updateAliens();

	comboTimer -= deltaTime;

	updatePowerUps();

	handleAlienCollisions();
	handlePlayerCollisions();
}

void Game::draw()
{
	renderWindow.clear();
	renderWindow.setView(mainView);
	renderWindow.draw(*field);

	drawScoreTags();
	drawPowerUps();
	drawBullets();
	drawAliens();

	player.draw(renderWindow);
	
	renderWindow.setView(hudView);
	hud.draw(renderWindow);
	renderWindow.setView(mainView);

	renderWindow.display();
}

void Game::manageViewUpdates()
{
	mainView.setCenter(player.getPosition());
	ajustCrossingWorldLimits();
	currentViewRectangle = FloatRect(
		mainView.getCenter() - mainView.getSize() / 2.0f,
		mainView.getSize()
	);
}

void Game::managePlayerUpdates()
{
	if (inputs.rotated) player.setRotation(inputs.aimAngle);
	player.update(inputs.move, deltaTime);
}

void Game::manageBulletUpdates()
{
	Bullet::updateRecoil(deltaTime);
	if (inputs.fire) fire();
	updateBullets();
}

void Game::fire()
{
	if (!player.isActive()) return;

	Bullet::shoot(
		player.getPosition(),
		player.getRotation(),
		player.isBoosted()
	);
}

void Game::onPlayerDeath()
{
	remainingLives--;
	manageGameOver();
	player.kill();
}

void Game::onAlienDeath(Alien& alien)
{
	alien.deactivate();

	rollPowerUp(alien);

	increaseScore();
	ScoreTag::spawn(alien.getPosition(), computeScoreIncrement());
}

void Game::handleAlienCollisions()
{
	for (int i = 0; i < ALIEN_COUNT; i++)
	{
		if (aliens[i].isActive() && !aliens[i].isSpawning())
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

			//	On ignore l'alien actuel de la boucle extérieure et on prend le prochain
			for (int j = i + 1; j < ALIEN_COUNT; j++)
			{
				if (aliens[j].isActive() && !aliens[j].isSpawning())
				{
					if (aliens[i].isCircleColliding(aliens[j]))
					{
						const Vector2f firstAlienPosition = aliens[i].getPosition();
						const Vector2f secondAlienPosition = aliens[j].getPosition();

						const float distance = Math::computeDistance(firstAlienPosition, secondAlienPosition);

						/*	Si les cercles sont en intersection, la distance entre les deux 
							est nécessairement plus petite que la somme de leur rayons.
							Ça veut dire que si on soustrait la distance à la somme des rayons,
							on aura la longueur de l'intersection entre les deux. */
						float overlapDistance = (aliens[i].getCollisionCircleRadius() + aliens[j].getCollisionCircleRadius()) - distance;

						if (overlapDistance > 0.0f)
						{
							/*	On veut mettre la correction dans la bonne direction,
								alors on trouve la position relative des deux aliens et
								on divise par la distance pour avoir un vecteur unitaire
								qui représente seulement la direction d'un envers l'autre. */
							Vector2f direction = (firstAlienPosition - secondAlienPosition) / distance;

							/*	On fait la répartition de la correction sur les deux aliens impliqués,
								donc on utilise la moitié de la distance d'intersection */
							Vector2f correction = direction * (overlapDistance * 0.5f);

							//	On applique la correction dans les sens opposés pour les deux aliens.
							aliens[i].move(correction);
							aliens[j].move(-correction);
						}
					}
				}
			}
		}
	}
}

void Game::handlePlayerCollisions()
{
	for (int i = 0; i < BOOST_COUNT; i++)
	{
		if (boosts[i].isActive())
			if (player.isCircleColliding(boosts[i])) onCollectBoost(boosts[i]);
	}

	for (int i = 0; i < NUKE_COUNT; i++)
	{
		if (nukes[i].isActive())
			if (player.isCircleColliding(nukes[i])) onCollectNuke(nukes[i]);
	}

	if (!player.isActive() || player.isInvincible()) return;

	for (int i = 0; i < ALIEN_COUNT; i++)
	{
		if (aliens[i].isActive() && !aliens[i].isSpawning())
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

void Game::increaseScore()
{
	const int lastScore = score;

	currentCombo++;
	if (comboTimer < 0.0f) currentCombo = 0;
	comboTimer = COMBO_DURATION;

	score += computeScoreIncrement();

	if (lastScore / LIFE_GAIN_SCORE_TRESHOLD < score / LIFE_GAIN_SCORE_TRESHOLD)
		remainingLives++;
}

unsigned int Game::computeScoreIncrement()
{
	return min(SCORE_INCREMENT + (currentCombo * COMBO_INCREMENT), MAX_SCORE_INCREMENT);
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
	const float roll = Math::generateRandomFloat(0, 100) / 100.0f;
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

void Game::onCollectBoost(Boost& boost)
{
	boost.collect();
	player.boost();
}

void Game::onCollectNuke(Nuke& nuke)
{
	nuke.collect();

	for (int i = 0; i < ALIEN_COUNT; i++)
	{
		if (aliens[i].isActive())
		{
			if (aliens[i].getGlobalBounds().findIntersection(currentViewRectangle).has_value())
				onAlienDeath(aliens[i]);
		}
	}
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

void Game::initAliens()
{
	Alien::setPlayer(&player);

	for (int i = 0; i < ALIEN_COUNT; i++)
		aliens[i].init(rand() % ContentPipeline::ALIEN_TEXTURE_NUMBER);
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

void Game::updateScoreTags()
{
	for (int i = 0; i < SCORE_TAG_COUNT; i++)
		scoreTags[i].update(deltaTime);
}

void Game::drawScoreTags()
{
	for (int i = 0; i < SCORE_TAG_COUNT; i++)
		scoreTags[i].draw(renderWindow);
}

void Game::managePause()
{
	if (inputs.pause)
	{
		isPaused = !isPaused;

		if (isPaused) music.pause();
		else music.play();
	}
}

void Game::manageGameOver()
{
	if (remainingLives == 0) {
		isGameOver = true;
		music.stop();
	}
}

bool Game::loadMusic()
{
	const unsigned int musicID = (int)floor(Math::generateRandomFloat(0, MUSIC_COUNT - 1));
	string musicPath;

	if (musicID == 0) musicPath = "Ressources\\Sounds\\Music\\Carpenter_brut_remorse.ogg";
	else if (musicID == 1) musicPath = "Ressources\\Sounds\\Music\\Deadmau5_welk_then.ogg";
	else musicPath = "Ressources\\Sounds\\Music\\Mega_drive_narc.ogg";

	if (!music.openFromFile(std::filesystem::path(musicPath))) return false;
	return true;
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

void Game::computeDeltaTime()
{
	deltaTime = clock.restart().asSeconds();
}

bool Game::unload()
{
	if (field != nullptr) delete field;

	return true;
}
