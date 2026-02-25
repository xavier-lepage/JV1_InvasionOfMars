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
		calculateDeltaTime();
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
	player.setPosition(WORLD_CENTER_X, WORLD_CENTER_Y);

	for (int i = 0; i < 3; i++)
	{
		aliens[i].init(i);
		aliens[i].setPosition(WORLD_CENTER_X - 200 + i * 200, WORLD_CENTER_Y + 200);
		aliens[i].activate();
	}

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
	}

	if (Joystick::isConnected(0))
	{
		//le / 100 à la fin: on ramène le tout à une échelle de 0 à 1: plus simple
		inputs.moveHorizontal = inputs.manageGamepadAxis(Joystick::getAxisPosition(0, Joystick::Axis::X));
		inputs.moveVertical = inputs.manageGamepadAxis(Joystick::getAxisPosition(0, Joystick::Axis::Y));
		inputs.isGamepadActive = true;
	}
	else
	{
		//Notez que les diagonales ne sont pas gérées actuellement et que le déplacement
		//est plus rapide en diagonale.  À vous de trouver une solution pour corriger ça.
		//Vous avez le droit d'ajouter une méthode dans la struct inputs qui ferait cette 
		//correction, un peu comme manageGamepadAxis.
		//Notez que si moveHorizontal et moveVectical sont toutes deux pas à 0, alors on est
		//en diagonal

		if (Keyboard::isKeyPressed(Keyboard::Scan::A)) inputs.moveHorizontal -= 1.0f;
		if (Keyboard::isKeyPressed(Keyboard::Scan::D)) inputs.moveHorizontal += 1.0f;
		if (Keyboard::isKeyPressed(Keyboard::Scan::W)) inputs.moveVertical -= 1.0f;
		if (Keyboard::isKeyPressed(Keyboard::Scan::S)) inputs.moveVertical += 1.0f;
	}
}

//Vous devrez centrer la vue sur le player: https://www.sfml-dev.org/tutorials/2.6/graphics-view-fr.php
void Game::update()
{
	//On peut déplacer la vue, mais on peut aussi lui la centrer sur une position précise, 
	//comme celle du joueur (avec la méthode setCenter).  Quand votre joueur va se déplacer 
	//vous devrez centrer la vue sur lui.
	mainView.move({ inputs.moveHorizontal * 5.0f, inputs.moveVertical * 5.0f });
	ajustCrossingWorldLimits();
}

void Game::draw()
{
	//Toujours important d'effacer l'écran précédent
	renderWindow.clear();
	renderWindow.setView(mainView);
	renderWindow.draw(*field);

	for (int i = 0; i < 3; i++)
		aliens[i].draw(renderWindow);

	player.draw(renderWindow);
	
	renderWindow.setView(hudView);
	hud.draw(renderWindow);
	renderWindow.setView(mainView);

	renderWindow.display();
}

bool Game::unload()
{
	if (field != nullptr) delete field;

	return true;
}

void Game::calculateDeltaTime()
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
