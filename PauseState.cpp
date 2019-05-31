#include "PauseState.h"
#include "GameState.h"
#include "MainMenuState.h"

#include <sstream>
#include <iostream>
#include <Windows.h>

namespace game {
	PauseState::PauseState(GameDataPtr data) : _data(data) {

	}

	void PauseState::init() {
		
		// Set textures
		this->_restartButton.setTexture(this->_data->assets.getTexture("Restart Button"));
		this->_resumeButton.setTexture(this->_data->assets.getTexture("Resume Button"));
		this->_homeButton.setTexture(this->_data->assets.getTexture("Home Button"));

		// Set origins
		this->_restartButton.setOrigin(sf::Vector2f((this->_restartButton.getGlobalBounds().width / 2), (this->_restartButton.getGlobalBounds().height / 2)));
		this->_resumeButton.setOrigin(sf::Vector2f((this->_resumeButton.getGlobalBounds().width / 2), (this->_resumeButton.getGlobalBounds().height / 2)));
		this->_homeButton.setOrigin(sf::Vector2f((this->_homeButton.getGlobalBounds().width / 2), (this->_homeButton.getGlobalBounds().height / 2)));

		// Scale sprites
		this->_restartButton.setScale(sf::Vector2f(SCREEN_HEIGHT * 0.25f / this->_restartButton.getGlobalBounds().width, SCREEN_HEIGHT * 0.25f / this->_restartButton.getGlobalBounds().height));
		this->_resumeButton.setScale(sf::Vector2f(SCREEN_HEIGHT * 0.25f / this->_resumeButton.getGlobalBounds().width, SCREEN_HEIGHT * 0.25f / this->_resumeButton.getGlobalBounds().height));
		this->_homeButton.setScale(sf::Vector2f(SCREEN_HEIGHT * 0.25f / this->_homeButton.getGlobalBounds().width, SCREEN_HEIGHT * 0.25f / this->_homeButton.getGlobalBounds().height));

		// Set default button scales
		_resumeScale = this->_resumeButton.getScale().x;
		_restartScale = this->_restartButton.getScale().x;
		_homeScale = this->_homeButton.getScale().x;

		// Set positions
		this->_restartButton.setPosition((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f));
		this->_resumeButton.setPosition((SCREEN_WIDTH * 0.5f - this->_restartButton.getGlobalBounds().width * 1.5f), (SCREEN_HEIGHT * 0.5f));
		this->_homeButton.setPosition((SCREEN_WIDTH * 0.5f + this->_restartButton.getGlobalBounds().width * 1.5f), (SCREEN_HEIGHT * 0.5f));

		// Initialize fade
		this->_opacity = 255;
		_fade.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));

		// Reset clocks
		this->_clock.restart();
		this->_fadeClock.restart();
	}

	void PauseState::handleInput() {
		sf::Event event;
		while (this->_data->window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::EventType::Closed:
				this->_data->window.close();
				break;

			case sf::Event::EventType::LostFocus:
				this->_data->assets.setVolume("Game Music", 0);
				break;

			case sf::Event::EventType::GainedFocus:
				this->_data->assets.setVolume("Game Music", 100);
				break;

			case sf::Event::EventType::MouseButtonReleased:
				if (sf::Mouse::Left == event.mouseButton.button) {
					
					// Resume button released
					if (this->_data->input.isMouseOver(this->_resumeButton, this->_data->window)) {
						this->_fadeClock.restart();

						while (this->_opacity < 255) {
							this->update(1.0f / 60.0f);
							this->draw(1.0f / 60.0f);
							if (this->_fadeClock.getElapsedTime().asSeconds() > 0.01 && this->_opacity < 255) {
								this->_opacity += 5;
								_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));
								this->_fadeClock.restart();
							}
							if (this->_opacity == 255) {
								Sleep(100);
								this->_data->machine.removeState();
							}
						}
					}

					// Restart button released
					if (this->_data->input.isMouseOver(this->_restartButton, this->_data->window)) {
						this->_fadeClock.restart();

						while (this->_opacity < 255) {
							this->update(1.0f / 60.0f);
							this->draw(1.0f / 60.0f);
							if (this->_fadeClock.getElapsedTime().asSeconds() > 0.01 && this->_opacity < 255) {
								this->_opacity += 5;
								_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));
								this->_fadeClock.restart();
							}
							if (this->_opacity == 255) {
								Sleep(100);
								this->_data->assets.stopSound("Game Music");
								this->resetFiles();
								this->_data->machine.addState(StatePtr(new GameState(_data)), true);
							}
						}
					}
				
					// Home button released
					if (this->_data->input.isMouseOver(this->_homeButton, this->_data->window)) {
						this->_fadeClock.restart();

						while (this->_opacity < 255) {
							this->update(1.0f / 60.0f);
							this->draw(1.0f / 60.0f);
							if (this->_fadeClock.getElapsedTime().asSeconds() > 0.01 && this->_opacity < 255) {
								this->_opacity += 5;
								_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));
								this->_fadeClock.restart();
							}
							if (this->_opacity == 255) {
								Sleep(100);
								this->_data->assets.stopSound("Game Music");
								this->_data->machine.addState(StatePtr(new MainMenuState(_data)), true);
							}
						}
					}
				}
				break;
			}
		}
	}

	void PauseState::update(float frameDif) {
		// Fade in
		if (this->_fadeClock.getElapsedTime().asSeconds() > 0.01 && this->_opacity > 0 && this->_clock.getElapsedTime().asSeconds() < 1) {
			this->_opacity -= 5;
			_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));
			this->_fadeClock.restart();
		}

		// Button animations
		if (this->_data->input.isMouseOver(_resumeButton, this->_data->window)) {
			if (this->_data->input.isSpriteClicked(_resumeButton, sf::Mouse::Left, this->_data->window)) {
				this->_resumeButton.setScale(_resumeScale, _resumeScale);
			}
			else if (this->_resumeButton.getScale().x < 1.2f * _resumeScale) {
				this->_resumeButton.scale(1.05f, 1.05f);
			}
		}

		if (!this->_data->input.isMouseOver(_resumeButton, this->_data->window)) {
			if (this->_resumeButton.getScale().x > _resumeScale) {
				this->_resumeButton.scale(0.95f, 0.95f);
			}
		}

		if (this->_data->input.isMouseOver(_restartButton, this->_data->window)) {
			if (this->_data->input.isSpriteClicked(_restartButton, sf::Mouse::Left, this->_data->window)) {
				this->_restartButton.setScale(_restartScale, _restartScale);
			}
			else if (this->_restartButton.getScale().x < 1.2f * _restartScale) {
				this->_restartButton.scale(1.05f, 1.05f);
			}
		}

		if (!this->_data->input.isMouseOver(_restartButton, this->_data->window)) {
			if (this->_restartButton.getScale().x > _restartScale) {
				this->_restartButton.scale(0.95f, 0.95f);
			}
		}

		if (this->_data->input.isMouseOver(_homeButton, this->_data->window)) {
			if (this->_data->input.isSpriteClicked(_homeButton, sf::Mouse::Left, this->_data->window)) {
				this->_homeButton.setScale(_homeScale, _homeScale);
			}
			else if (this->_homeButton.getScale().x < 1.2f * _homeScale) {
				this->_homeButton.scale(1.05f, 1.05f);
			}
		}

		if (!this->_data->input.isMouseOver(_homeButton, this->_data->window)) {
			if (this->_homeButton.getScale().x > _homeScale) {
				this->_homeButton.scale(0.95f, 0.95f);
			}
		}
	}

	void PauseState::draw(float frameDif) {
		this->_data->window.clear(sf::Color(130, 187, 228));

		this->_data->window.draw(this->_restartButton);
		this->_data->window.draw(this->_resumeButton);
		this->_data->window.draw(this->_homeButton);
		
		this->_data->window.draw(this->_fade);

		this->_data->window.display();
	}

	void PauseState::resetFiles() {
		// Reset score
		std::vector<int> vector;
		for (int i = 0; i < this->_data->files.readFile("res/data/Score.txt").size(); i++) {
			vector.push_back(0);
		}

		this->_data->files.writeFile("res/data/Score.txt", vector);

		// Reset array
		vector.clear();
		for (int i = 0; i < this->_data->files.readFile("res/data/Array.txt").size(); i++) {
			vector.push_back(0);
		}

		this->_data->files.writeFile("res/data/Array.txt", vector);

		// Reset prev
		this->_data->files.writeFile("res/data/Prev.txt", vector);
	}
}