#include "AssetManager.h"
#include <iostream>

namespace game {
	void AssetManager::loadTexture(std::string name, std::string fileName) {
		sf::Texture tex;

		if (tex.loadFromFile(fileName)) {
			this->_textures[name] = tex;
		}
	}

	void AssetManager::loadFont(std::string name, std::string fileName) {
		sf::Font font;

		if (font.loadFromFile(fileName)) {
			this->_fonts[name] = font;
		}
	}

	sf::Texture& AssetManager::getTexture(std::string name) {
		return this->_textures.at(name);
	}

	sf::Font& AssetManager::getFont(std::string name) {
		return this->_fonts.at(name);
	}

	void AssetManager::loadSound(std::string name, std::string fileName) {
		sf::SoundBuffer buffer;
		sf::Sound sound;

		if (buffer.loadFromFile(fileName)) {
			this->_buffers[name] = buffer;
			sound.setBuffer(this->_buffers[name]);
			this->_sounds[name] = sound;
		}
		else std::cout << "Failed to load audio buffer : " << name << std::endl;
	}

	void AssetManager::playSound(std::string name, bool loop) {
		this->_sounds[name].setLoop(loop);
		this->_sounds[name].play();

		if (this->_sounds[name].getVolume() < 100) {
			this->_sounds[name].setVolume(100);
		}
	}

	void AssetManager::pauseSound(std::string name) {
		this->_sounds[name].pause();
	}

	void AssetManager::stopSound(std::string name) {
		sf::Clock clock;
		int volume = this->_sounds[name].getVolume();

		while (!this->_sounds[name].getVolume() == 0) {
			if (clock.getElapsedTime().asSeconds() > 0.1f) {
				this->_sounds[name].setVolume(volume = volume - 10);
			}
		}
		this->_sounds[name].stop();
	}

	sf::Sound::Status AssetManager::getSoundStatus(std::string name) {
		return this->_sounds[name].getStatus();
	}

	void AssetManager::setVolume(std::string name, int volume) {
		this->_sounds[name].setVolume(volume);
	}
}