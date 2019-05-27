#include "MainMenuState.h"
#include "GameState.h"
#include "DEFINITIONS.h"

#include <sstream>
#include <iostream>
#include <Windows.h>

namespace game {
	MainMenuState::MainMenuState(GameDataPtr data) : _data(data) {

	}

	void MainMenuState::init() {
		sf::CircleShape gear;
		_gears.push_back(gear);
		_gears.push_back(gear);
		_gears.push_back(gear);

		this->_gears[0].setRadius(350);
		this->_gears[1].setRadius(150);
		this->_gears[2].setRadius(450);
		
		this->_title.setTexture(this->_data->assets.getTexture("Game Title"));
		this->_playButton.setTexture(this->_data->assets.getTexture("Play Button"));
		this->_exitButton.setTexture(this->_data->assets.getTexture("Exit Button"));
		this->_gears[0].setTexture(&this->_data->assets.getTexture("Gear"));
		this->_gears[1].setTexture(&this->_data->assets.getTexture("Gear"));
		this->_gears[2].setTexture(&this->_data->assets.getTexture("Gear"));

		this->_title.setOrigin(sf::Vector2f((this->_title.getGlobalBounds().width / 2.0f), (this->_title.getGlobalBounds().height / 2.0f)));
		this->_playButton.setOrigin(sf::Vector2f((this->_playButton.getGlobalBounds().width / 2.0f), (this->_playButton.getGlobalBounds().height / 2.0f)));
		this->_exitButton.setOrigin(sf::Vector2f((this->_exitButton.getGlobalBounds().width / 2.0f), (this->_exitButton.getGlobalBounds().height / 2.0f)));
		this->_gears[0].setOrigin(sf::Vector2f((this->_gears[0].getGlobalBounds().width / 2.0f), (this->_gears[0].getGlobalBounds().height / 2.0f)));
		this->_gears[1].setOrigin(sf::Vector2f((this->_gears[1].getGlobalBounds().width / 2.0f), (this->_gears[1].getGlobalBounds().height / 2.0f)));
		this->_gears[2].setOrigin(sf::Vector2f((this->_gears[2].getGlobalBounds().width / 2.0f), (this->_gears[2].getGlobalBounds().height / 2.0f)));

		this->_title.setPosition((SCREEN_WIDTH / 2.0f), (this->_title.getGlobalBounds().height * 0.8f));
		this->_playButton.setPosition(((SCREEN_WIDTH / 2.0f) + this->_playButton.getGlobalBounds().width), ((SCREEN_HEIGHT / 2.0f) + this->_playButton.getGlobalBounds().height * 0.8f));
		this->_exitButton.setPosition(((SCREEN_WIDTH / 2.0f) - this->_exitButton.getGlobalBounds().width), ((SCREEN_HEIGHT / 2) + this->_exitButton.getGlobalBounds().height * 0.8f));
		this->_gears[0].setPosition(this->_gears[0].getGlobalBounds().width * 0.15f, SCREEN_HEIGHT + this->_gears[0].getGlobalBounds().height*0.2f);
		this->_gears[1].setPosition(this->_gears[1].getGlobalBounds().width * (-0.15f), this->_gears[1].getGlobalBounds().height*0.2f);
		this->_gears[2].setPosition(SCREEN_WIDTH + this->_gears[2].getGlobalBounds().width * 0.2f, SCREEN_HEIGHT/2.0f + this->_gears[2].getGlobalBounds().height*0.2f);

		// Initialize fade
		this->_opacity = 255;
		_fade.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));

		// Reset clocks
		this->_clock.restart();
		this->_fadeClock.restart();
	}

	void MainMenuState::handleInput() {
		sf::Event event;
		while (this->_data->window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::EventType::Closed:
				this->_data->window.close();
				break;

			case sf::Event::EventType::LostFocus:
				this->_data->assets.setVolume("Menu Music", 0);
				break;

			case sf::Event::EventType::GainedFocus:
				this->_data->assets.setVolume("Menu Music", 100);
				break;

			case sf::Event::EventType::MouseButtonReleased:
				// Exit button released
				if (sf::Mouse::Left == event.mouseButton.button) {
					if (this->_data->input.isMouseOver(this->_exitButton, this->_data->window)) {
						this->_exitClock.restart();
						this->_fadeClock.restart();

						while (this->_exitClock.getElapsedTime().asSeconds() < 1.5) {
							if (this->_fadeClock.getElapsedTime().asSeconds() > 0.01 && this->_opacity < 255) {
								this->_opacity += 5;
								_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));
								this->_fadeClock.restart();
							}
							this->update(1.0f / 60.0f);
							this->draw(1.0f / 60.0f);
							if (this->_opacity == 255) {
								this->_data->window.close();
							}
						}
					}
					
					// Play button released
					if (this->_data->input.isMouseOver(this->_playButton, this->_data->window)) {
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
								this->_data->assets.stopSound("Menu Music");
								this->_data->machine.addState(StatePtr(new GameState(_data)), true);
							}
						}
					}
				}
			}
		}
	}

	void MainMenuState::update(float frameDif) {
		// Fade in
		if (this->_fadeClock.getElapsedTime().asSeconds() > 0.01 && this->_opacity > 0 && this->_clock.getElapsedTime().asSeconds() < 1) {
			this->_opacity -= 5;
			_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));
			this->_fadeClock.restart();
		}

		// Play music
		if (this->_clock.getElapsedTime().asSeconds() > 0.2 && this->_clock.getElapsedTime().asSeconds() < 0.6) {
			if (this->_data->assets.getSoundStatus("Menu Music") == sf::Sound::Playing) {}
			else this->_data->assets.playSound("Menu Music", true);
		}

		//Rotate gears
		if (this->_gearClock.getElapsedTime().asSeconds() > 0.01) {
			this->_gears[0].rotate(0.1f);
			this->_gears[1].rotate(0.4f);
			this->_gears[2].rotate(0.03f);
			this->_gearClock.restart();
		}
		
		// Button animations
		if (this->_data->input.isMouseOver(_playButton, this->_data->window)) {
			if (this->_data->input.isSpriteClicked(_playButton, sf::Mouse::Left, this->_data->window)) {
				this->_playButton.setScale(1.0f, 1.0f);
			}
			else if (this->_playButton.getScale().x < 1.2f) {
				this->_playButton.scale(1.05f, 1.05f);
			}
		}
		
		if (!this->_data->input.isMouseOver(_playButton, this->_data->window)) {
			if (this->_playButton.getScale().x > 1.0f) {
				this->_playButton.scale(0.95f, 0.95f);
			}
		}
		
		if (this->_data->input.isMouseOver(_exitButton, this->_data->window)) {
			if (this->_data->input.isSpriteClicked(_exitButton, sf::Mouse::Left, this->_data->window)) {
				this->_exitButton.setScale(1.0f, 1.0f);
			}
		else if (this->_exitButton.getScale().x < 1.2f) {
				this->_exitButton.scale(1.05f, 1.05f);
			}
		}

		if (!this->_data->input.isMouseOver(_exitButton, this->_data->window)) {
			if (this->_exitButton.getScale().x > 1.0f) {
				this->_exitButton.scale(0.95f, 0.95f);
			}
		}
	}

	void MainMenuState::draw(float frameDif) {
		this->_data->window.clear(sf::Color(20, 66, 99));

		this->_data->window.draw(this->_title);
		this->_data->window.draw(this->_playButton);
		this->_data->window.draw(this->_exitButton);
		this->_data->window.draw(this->_gears[0]);
		this->_data->window.draw(this->_gears[1]);
		this->_data->window.draw(this->_gears[2]);
		this->_data->window.draw(this->_fade);

		this->_data->window.display();
	}
}