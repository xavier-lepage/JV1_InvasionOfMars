#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

class ContentPipeline
{
public:
	bool loadContent();
	const Texture& getBackgroundTexture() const;
	const Texture& getPlayerTexture() const;
	const Texture& getLogoTexture(const int logoNumber) const;
	const Texture& getProjectileTexture(const int projectileNumber) const;
	const Texture& getAlienTexture(const int alienTextureNumber) const;

	const SoundBuffer& ContentPipeline::getHordeBuffer() const;
	const SoundBuffer& ContentPipeline::getAlienDeathBuffer() const;
	const SoundBuffer& ContentPipeline::getPlayerDeathBuffer() const;
	const SoundBuffer& ContentPipeline::getShotBuffer() const;
	const SoundBuffer& ContentPipeline::getBoostedShotBuffer() const;
	const SoundBuffer& ContentPipeline::getTokenSoundBuffer() const;
	const SoundBuffer& ContentPipeline::getExplosionSoundBuffer() const;

	const Font& getFont() const;

	static ContentPipeline& getInstance();

	static const int ALIEN_TEXTURE_NUMBER = 3;
	static const int PROJECTILE_TEXTURE_NUMBER = 2;
	static const int LOGO_TEXTURE_NUMBER = 2;

private:
	ContentPipeline() {}
	ContentPipeline(ContentPipeline& other) {}
	void operator=(const ContentPipeline&) {}
	static ContentPipeline instance;

	Texture backgroundTexture;
	Texture playerTexture;
	Texture alienTexture[ALIEN_TEXTURE_NUMBER];
	Texture projectileTexture[PROJECTILE_TEXTURE_NUMBER];
	Texture logoTexture[LOGO_TEXTURE_NUMBER];

	SoundBuffer hordeBuffer;
	SoundBuffer alienDeathBuffer;
	SoundBuffer playerDeathBuffer;
	SoundBuffer shotBuffer;
	SoundBuffer boostedShotBuffer;
	SoundBuffer tokenSoundBuffer;
	SoundBuffer explosionSoundBuffer;

	Font font;
};