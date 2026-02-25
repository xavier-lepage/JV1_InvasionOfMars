#include "ContentPipeline.h"

ContentPipeline ContentPipeline::instance;

ContentPipeline& ContentPipeline::getInstance()
{
	return instance;
}

bool ContentPipeline::loadContent()
{
	if (!backgroundTexture.loadFromFile("Ressources\\Sprites\\Backgrounds\\MoonReflectiveBackground.png")) return false;
	
	if (!playerTexture.loadFromFile("Ressources\\Sprites\\SpacemanAndAliens\\Spaceman.png")) return false;

	if (!alienTexture[0].loadFromFile("Ressources\\Sprites\\SpacemanAndAliens\\Alien1.png")) return false;
	if (!alienTexture[1].loadFromFile("Ressources\\Sprites\\SpacemanAndAliens\\Alien2.png")) return false;
	if (!alienTexture[2].loadFromFile("Ressources\\Sprites\\SpacemanAndAliens\\Alien3.png")) return false;

	if (!logoTexture[0].loadFromFile("Ressources\\Sprites\\Logos\\BoostLogo.png")) return false;
	if (!logoTexture[1].loadFromFile("Ressources\\Sprites\\Logos\\NukeLogo.png")) return false;

	if (!projectileTexture[0].loadFromFile("Ressources\\Sprites\\Projectiles\\Blast.png")) return false;
	if (!projectileTexture[1].loadFromFile("Ressources\\Sprites\\Projectiles\\Bullet.png")) return false;

	if (!hordeBuffer.loadFromFile("Ressources\\Sounds\\Characters\\Horde.ogg")) return false;
	if (!alienDeathBuffer.loadFromFile("Ressources\\Sounds\\Characters\\AlienDeath.ogg")) return false;
	if (!playerDeathBuffer.loadFromFile("Ressources\\Sounds\\Characters\\PlayerDeath.ogg")) return false;
	if (!shotBuffer.loadFromFile("Ressources\\Sounds\\SpaceShots\\Shot01.ogg")) return false;
	if (!boostedShotBuffer.loadFromFile("Ressources\\Sounds\\SpaceShots\\Shot02.ogg")) return false;
	if (!tokenSoundBuffer.loadFromFile("Ressources\\Sounds\\SpaceShots\\ToggleSound.ogg")) return false;
	if (!explosionSoundBuffer.loadFromFile("Ressources\\Sounds\\SpaceShots\\Explosion.ogg")) return false;

	if (!font.openFromFile("Ressources\\Fonts\\segoepr.ttf")) return false;

	return true;
}

const Texture& ContentPipeline::getBackgroundTexture() const { return backgroundTexture; }
const Texture& ContentPipeline::getPlayerTexture() const { return playerTexture; }
const Texture& ContentPipeline::getLogoTexture(const int logoNumber) const { return logoTexture[logoNumber]; }
const Texture& ContentPipeline::getProjectileTexture(const int projectileNumber) const { return projectileTexture[projectileNumber]; }
const Texture& ContentPipeline::getAlienTexture(const int alienTextureNumber) const { return alienTexture[alienTextureNumber]; }

const SoundBuffer& ContentPipeline::getHordeBuffer() const { return hordeBuffer; }
const SoundBuffer& ContentPipeline::getAlienDeathBuffer() const { return alienDeathBuffer; }
const SoundBuffer& ContentPipeline::getPlayerDeathBuffer() const { return playerDeathBuffer; }
const SoundBuffer& ContentPipeline::getShotBuffer() const { return shotBuffer; }
const SoundBuffer& ContentPipeline::getBoostedShotBuffer() const { return boostedShotBuffer; }
const SoundBuffer& ContentPipeline::getTokenSoundBuffer() const { return tokenSoundBuffer; }
const SoundBuffer& ContentPipeline::getExplosionSoundBuffer() const { return explosionSoundBuffer; }

const Font& ContentPipeline::getFont() const { return font; }
