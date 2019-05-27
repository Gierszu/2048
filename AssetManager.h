#pragma once

#include <string>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace game {
	class AssetManager
	{
	public:
		AssetManager() {}
		~AssetManager() {}

		void loadTexture(std::string name, std::string fileName);
		sf::Texture& getTexture(std::string name);

		void loadFont(std::string name, std::string fileName);
		sf::Font& getFont(std::string name);
		
		void loadSound(std::string name, std::string fileName);
		
		void playSound(std::string name, bool loop = false);
		void pauseSound(std::string name);
		void stopSound(std::string name);
		void setVolume(std::string name, int volume);
		sf::Sound::Status getSoundStatus(std::string name);

	private:
		std::map<std::string, sf::Texture> _textures;
		std::map<std::string, sf::Font> _fonts;
		std::map<std::string, sf::SoundBuffer> _buffers;
		std::map<std::string, sf::Sound> _sounds;
	};
}

