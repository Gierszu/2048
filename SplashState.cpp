#include "SplashState.h"
#include <sstream>
#include <iostream>
#include "DEFINITIONS.h"
#include"MainMenuState.h"

namespace game {
	SplashState::SplashState(GameDataPtr data) : _data(data) {

	}

	void SplashState::init() {
		this->_data->assets.loadTexture("Logo", LOGO_FILEPATH);

		_logo.setTexture(this->_data->assets.getTexture("Logo"));
		_logo.setOrigin(_logo.getGlobalBounds().width / 2, _logo.getGlobalBounds().height / 2);
		_logo.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

		_fade.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		_fade.setFillColor(sf::Color(0, 0, 0, 255));

		this->_data->assets.loadSound("Intro", INTRO_FILEPATH);

		// Load menu things
		{
			// Menu textures
			this->_data->assets.loadTexture("Game Title", MAIN_MENU_TITLE_FILEPATH);
			this->_data->assets.loadTexture("Play Button", PLAY_BUTTON);
			this->_data->assets.loadTexture("Exit Button", EXIT_BUTTON);
			this->_data->assets.loadTexture("Gear", GEAR_FILEPATH);

			//Menu music
			this->_data->assets.loadSound("Menu Music", MENU_THEME);
		}

		// Load game things
		{
			// Load TILEs
			this->_data->assets.loadTexture("Back", GRID_BACK);
			this->_data->assets.loadTexture("0", TILE_0);
			this->_data->assets.loadTexture("2", TILE_2);
			this->_data->assets.loadTexture("4", TILE_4);
			this->_data->assets.loadTexture("8", TILE_8);
			this->_data->assets.loadTexture("16", TILE_16);
			this->_data->assets.loadTexture("32", TILE_32);
			this->_data->assets.loadTexture("64", TILE_64);
			this->_data->assets.loadTexture("128", TILE_128);
			this->_data->assets.loadTexture("256", TILE_256);
			this->_data->assets.loadTexture("512", TILE_512);
			this->_data->assets.loadTexture("1024", TILE_1024);
			this->_data->assets.loadTexture("2048", TILE_2048);

			// Load buttons
			this->_data->assets.loadTexture("Pause", PAUSE_BUTTON);
			this->_data->assets.loadTexture("Prev", PREV_BUTTON);

			// Load music
			this->_data->assets.loadSound("Game Music", GAME_MUSIC);

			// Load text boxes
			this->_data->assets.loadTexture("Text Box", TEXT_BOX);

			// Load fonts
			this->_data->assets.loadFont("Calm Font", CALM_FONT);

			// Load Over boxes
			this->_data->assets.loadTexture("Lose Box", LOSE_BOX);
			this->_data->assets.loadTexture("Win Box", WIN_BOX);
		}

		// Load pause things
		{
			//Pause textures
			this->_data->assets.loadTexture("Restart Button", RESTART_BUTTON);
			this->_data->assets.loadTexture("Resume Button", RESUME_BUTTON);
			this->_data->assets.loadTexture("Home Button", HOME_BUTTON);
		}

		// Reset clock
		this->_clock.restart();
	}

	void SplashState::handleInput() {
		sf::Event event;
		while (this->_data->window.pollEvent(event)) {
			if (sf::Event::Closed == event.type) {
				this->_data->window.close();
			}
		}
	}

	void SplashState::update(float frameDif) {
		if (this->_clock.getElapsedTime().asSeconds() > SPLASH_STATE_SHOW_TIME) {
			this->_data->machine.addState(StatePtr(new MainMenuState(_data)), true);
		}
		
		if (this->_fadeClock.getElapsedTime().asSeconds() > 0.01 && this->_opacity > 0 && this->_clock.getElapsedTime().asSeconds() < 1) {
			this->_opacity -= 5;
			_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));
			this->_fadeClock.restart();
		}

		if (this->_fadeClock.getElapsedTime().asSeconds() > 0.01 && this->_opacity < 255 && this->_clock.getElapsedTime().asSeconds() > SPLASH_STATE_SHOW_TIME - 1) {
			this->_opacity += 5;
			_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));
			this->_fadeClock.restart();
		}

		if (this->_clock.getElapsedTime().asSeconds() > 0.4 && this->_clock.getElapsedTime().asSeconds() < 0.5) {
			this->_data->assets.playSound("Intro");
		}
	}

	void SplashState::draw(float frameDif) {
		this->_data->window.clear(sf::Color(235, 235, 235));

		this->_data->window.draw(this->_logo);
		this->_data->window.draw(this->_fade);

		this->_data->window.display();
	}
}
