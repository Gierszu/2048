#include "Game.h"
#include "SplashState.h"

namespace game {
	Game::Game(int width, int height, std::string title) {
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;

		_data->window.setKeyRepeatEnabled(false);

		_data->window.create(sf::VideoMode(width, height), title, sf::Style::None, settings);

		_data->machine.addState(StatePtr(new SplashState(this->_data)));

		this->Run();
	}

	void Game::Run() {
		float newTime, frameTime, interpolation;

		float currentTime = this->_clock.getElapsedTime().asSeconds();
		float accumulator = 0.0f;

		while (this->_data->window.isOpen()) {
			this->_data->machine.processStateChanges();
			newTime = this->_clock.getElapsedTime().asSeconds();
			frameTime = newTime - currentTime;

			if (frameTime > 0.25f) {
				frameTime = 0.25f;
			}

			currentTime = newTime;
			accumulator += frameTime;

			while (accumulator >= frameDif) {
				this->_data->machine.getActiveState()->handleInput();
				this->_data->machine.getActiveState()->update(frameDif);

				accumulator -= frameDif;
			}

			interpolation = accumulator / frameDif;
			this->_data->machine.getActiveState()->draw(interpolation);
		}
	}
}