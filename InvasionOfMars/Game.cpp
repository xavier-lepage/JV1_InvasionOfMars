#include "Game.h"
#include "ContentPipeline.h"

Game::Game()
{
	renderWindow.create(VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }, 32), "Invasion of Mars");// , Style::None, State::Fullscreen);
	
	
	mainView = View(FloatRect({ 0.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT }));
	mainView.setCenter({ WORLD_WIDTH / 2.0f , WORLD_HEIGHT / 2.0f });

	hudView = View(FloatRect({ 0.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT }));

	//Synchonisation coordonnée à l'écran!  Normalement 60 frames par secondes. À faire absolument
	//renderWindow.setVerticalSyncEnabled(true);  //De plus en plus d'écrans sont rafraichis à plus de 60 frames par seconde, alors attention.
	renderWindow.setFramerateLimit(60);  //Équivalent... normalement, mais pas toujours. À utiliser si la synchonisation de l'écran fonctionne mal.
	//https://www.sfml-dev.org/tutorials/3.0/window/window/#controlling-the-framerate

	renderWindow.setKeyRepeatEnabled(false);
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
	player.setCollisionCircleRadius(PLAYER_RADIUS);

	topBound = player.getTexture().getSize().y / 2.0f;
	bottomBound = WORLD_HEIGHT - player.getTexture().getSize().y / 2.0f;
	rightBound = WORLD_WIDTH - player.getTexture().getSize().x / 2.0f;
	leftBound = player.getTexture().getSize().x / 2.0f;

	initBullets();
	initAliens();

	hud.hudInit();	

	player.activate();
	music.setLooping(true);
	music.play();
	return true;
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
			if (joystickButtonPressed->button == 7) inputs.pause = true;
		}

		if (Joystick::isConnected(0)) continue;

		if (const Event::KeyPressed* keyPressed = event->getIf<Event::KeyPressed>())
		{
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

	if (isPaused || isGameOver) return;

	currentViewRectangle = FloatRect(
		mainView.getCenter() - mainView.getSize() / 2.0f,
		mainView.getSize()
	);

	//On peut déplacer la vue, mais on peut aussi lui la centrer sur une position précise, 
	//comme celle du joueur (avec la méthode setCenter).  Quand votre joueur va se déplacer 
	//vous devrez centrer la vue sur lui.
	player.move({ inputs.move.x * PLAYER_SPEED * deltaTime, inputs.move.y * PLAYER_SPEED * deltaTime });
	if (inputs.rotated) player.setRotation(inputs.aimAngle);
	keepPlayerInbound();
	player.update(deltaTime);

	if (recoilTimer > 0.0f) recoilTimer -= deltaTime;
	if (inputs.fire) fire();
	updateBullets();

	mainView.setCenter(player.getPosition());
	ajustCrossingWorldLimits();

	if (alienSpawnTimer > 0.0f) alienSpawnTimer -= deltaTime;
	spawnAliens();
	updateAliens();
	comboTimer -= deltaTime;

	handleProjectileCollisions();
	handlePlayerCollisions();
}

void Game::draw()
{
	renderWindow.clear();
	renderWindow.setView(mainView);
	renderWindow.draw(*field);

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

void Game::onPlayerDeath()
{
	if (!player.isActive() || player.isInvincible()) return;

	remainingLives--;
	manageGameOver();
	player.kill();
}

void Game::handleProjectileCollisions()
{
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		if (bullets[i].isActive())
		{
			for (int j = 0; j < ALIEN_COUNT; j++)
			{
				if (aliens[j].isActive())
				{
					if (bullets[i].isCircleColliding(aliens[j]))
						onAlienDeath(bullets[i], aliens[j]);
				}
			}
		}
	}
}

void Game::onAlienDeath(Bullet& bullet, Alien& alien)
{
	bullet.deactivate();
	alien.deactivate();

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
	for (int i = 0; i < ALIEN_COUNT; i++)
	{
		if (aliens[i].isActive())
			if (player.isCircleColliding(aliens[i])) onPlayerDeath();
	}
}

void Game::initBullets()
{
	for (int i = 0; i < BULLET_COUNT; i++)
		bullets[i].setTexture(ContentPipeline::getInstance().getProjectileTexture(BULLET_TEXTURE_ID));
}

void Game::updateBullets()
{
	for (int i = 0; i < BULLET_COUNT; i++)
		bullets[i].update(deltaTime, currentViewRectangle);
}

void Game::drawBullets()
{
	for (int i = 0; i < BULLET_COUNT; i++)
		bullets[i].draw(renderWindow);
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

void Game::keepPlayerInbound()
{
	if (player.getPosition().x < leftBound) player.setPosition(leftBound, player.getPosition().y);
	if (player.getPosition().y < topBound) player.setPosition(player.getPosition().x, topBound);

	if (player.getPosition().x > rightBound) player.setPosition(rightBound, player.getPosition().y);
	if (player.getPosition().y > bottomBound) player.setPosition(player.getPosition().x, bottomBound);
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
