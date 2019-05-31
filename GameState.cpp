#include "GameState.h"
#include "PauseState.h"
#include "DEFINITIONS.h"
#include "MainMenuState.h"

#include <sstream>
#include <iostream>
#include <Windows.h>
#include <random>
#include <ctime>

namespace game {
	GameState::GameState(GameDataPtr data) : _data(data) {

	}

	void GameState::init() {
		_gameState = STATE_PLAYING;

		// Set textures
		this->_title.setTexture(this->_data->assets.getTexture("Game Title"));
		this->_pauseButton.setTexture(this->_data->assets.getTexture("Pause"));
		this->_prevButton.setTexture(this->_data->assets.getTexture("Prev"));

		// Set origins
		this->_title.setOrigin(sf::Vector2f((this->_title.getGlobalBounds().width / 2.0f), (this->_title.getGlobalBounds().height / 2.0f)));
		this->_pauseButton.setOrigin(sf::Vector2f((this->_pauseButton.getGlobalBounds().width / 2), (this->_pauseButton.getGlobalBounds().height / 2)));
		this->_prevButton.setOrigin(sf::Vector2f((this->_prevButton.getGlobalBounds().width / 2), (this->_prevButton.getGlobalBounds().height / 2)));
		
		// Scale sprites
		this->_title.setScale(sf::Vector2f(0.5f, 0.5f));
		this->_pauseButton.setScale(sf::Vector2f((SCREEN_HEIGHT * 0.9f / 8.0f) / (_pauseButton.getGlobalBounds().width), (SCREEN_HEIGHT * 0.9f / 8.0f) / (_pauseButton.getGlobalBounds().height)));
		this->_prevButton.setScale(sf::Vector2f((SCREEN_HEIGHT * 0.9f / 8.0f) / (_prevButton.getGlobalBounds().width), (SCREEN_HEIGHT * 0.9f / 8.0f) / (_prevButton.getGlobalBounds().height)));

		_pauseScale = this->_pauseButton.getScale().x;
		_prevScale = this->_prevButton.getScale().x;

		// Set positions
		this->_pauseButton.setPosition(sf::Vector2f(SCREEN_WIDTH - this->_pauseButton.getGlobalBounds().width * 0.7, this->_pauseButton.getGlobalBounds().height * 0.7));
		this->_prevButton.setPosition(sf::Vector2f(SCREEN_WIDTH - this->_pauseButton.getGlobalBounds().width * 0.7f - this->_prevButton.getGlobalBounds().width * 1.3f, this->_prevButton.getGlobalBounds().height * 0.7));
		this->_title.setPosition(sf::Vector2f(this->_title.getGlobalBounds().width * 0.7f , this->_title.getGlobalBounds().height * 0.7f));

		// Set text boxes
		this->_scoreBoard.init(_data, sf::Vector2f(0.9f * _title.getGlobalBounds().width, 0.7f * _title.getGlobalBounds().height), sf::Vector2f(_title.getPosition().x, 2.5f * _title.getPosition().y));
		this->_highscoreBoard.init(_data, sf::Vector2f(_scoreBoard.getGlobalBounds().width, _scoreBoard.getGlobalBounds().height), sf::Vector2f(_scoreBoard.getPosition().x, 1.5f * _scoreBoard.getPosition().y));
		_scoreBoard.changeTitle("Score");
		_scoreBoard.changeText(std::to_string(this->_data->files.readSingleFile("res/data/Score.txt")));
		_highscoreBoard.changeTitle("Highscore");
		_highscoreBoard.changeText(std::to_string(this->_data->files.readSingleFile("res/data/Highscore.txt")));
		_highscore = this->_data->files.readSingleFile("res/data/Highscore.txt");
		_score = this->_data->files.readFile("res/data/Score.txt")[0];
		_prevScore = this->_data->files.readFile("res/data/Score.txt")[1];

		// Initialize grid
		_numberOfTiles = 4;
		std::vector<int> avector = this->_data->files.readFile("res/data/Array.txt");
		std::vector<std::vector<int>> Array;
		int i = 0;
		for (int y = 0; y < _numberOfTiles; y++) {
			std::vector<int> vector;
			for (int x = 0; x < _numberOfTiles; x++) {
				vector.push_back(avector[i]);
				i++;
			}
			Array.push_back(vector);
		}
		i = 0;
		std::vector<int> pvector = this->_data->files.readFile("res/data/Prev.txt");
		std::vector<std::vector<int>> Prev;
		for (int y = 0; y < _numberOfTiles; y++) {
			std::vector<int> vector;
			for (int x = 0; x < _numberOfTiles; x++) {
				vector.push_back(pvector[i]);
				i++;
			}
			Prev.push_back(vector);
		}
		i = 0;

		this->_grid.init(this->_data, sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f), SCREEN_HEIGHT * 0.9f, _numberOfTiles, Array, Prev);

		// Initialize fade
		this->_opacity = 255;
		_fade.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));

		// Initialize game tiles
		if (_grid.isArrayEmpty()) {
			std::thread initializeTiles(&Grid::initTiles, &_grid);
			initializeTiles.detach();

			for (int i = 0; i < 20; i++) {
				_grid.animate(20);
				this->update(1.0f / 60.0f);
				this->draw(1.0f / 60.0f);
				Sleep(1 / 20);
			}
		}
		_grid.addToScore(_score);

		_grid.moveToTiles();
		_grid.setTiles();
		this->checkIfOver();

	}

	void GameState::handleInput() {
		if (_gameState == STATE_PAUSED) {
			_gameState = _overBox.handleInput();
		}
		
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
				if (sf::Mouse::Left == event.mouseButton.button && _gameState == STATE_PLAYING) {
					// Prev button released
					if (this->_data->input.isMouseOver(this->_prevButton, this->_data->window)) {
						_grid.addToScore(_prevScore - _score);
						_score = _prevScore;
						_grid.setArrToPrev();
						_grid.moveToTiles();
						_grid.setTiles();

						this->updateScore();
						this->updateHighscore();
						this->updateFiles();
					}

					// Pause button released
					if (this->_data->input.isMouseOver(this->_pauseButton, this->_data->window)) {
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
								this->_data->machine.addState(StatePtr(new PauseState(_data)), false);
							}
						}
					}
				}
				break;
			case sf::Event::EventType::KeyPressed:
				// Keyboard left
				if (sf::Keyboard::Left == event.key.code && _grid.checkIfPossible(Direction::LEFT) && _gameState == STATE_PLAYING) {
					_prevScore = _score;
					_grid.execute(Direction::LEFT);

					sf::Clock clock;
					for (int i = 0; i < 10; i++) {
						_grid.animate(10);
						this->update(1.0f / 60.0f);
						this->draw(1.0f / 60.0f);
						float time = clock.getElapsedTime().asMilliseconds();
						if (time < (100 / 10)) {
							Sleep((100 - time) / 10);
						}
					}

					_grid.moveToTiles();

					this->updateScore();
					this->updateHighscore();
					this->updateFiles();
					this->checkIfOver();
				}

				// Keyboard right
				if (sf::Keyboard::Right == event.key.code && _grid.checkIfPossible(Direction::RIGHT) && _gameState == STATE_PLAYING) {
					_prevScore = _score;
					_grid.execute(Direction::RIGHT);

					sf::Clock clock;
					for (int i = 0; i < 10; i++) {
						clock.restart();
						_grid.animate(10);
						this->update(1.0f / 60.0f);
						this->draw(1.0f / 60.0f);
						float time = clock.getElapsedTime().asMilliseconds();
						if (time < (100 / 10)) {
							Sleep((100 - time) / 10);
						}
					}

					_grid.moveToTiles();

					this->updateScore();
					this->updateHighscore();
					this->updateFiles();
					this->checkIfOver();
				}

				// Keyboard up
				if (sf::Keyboard::Up == event.key.code && _grid.checkIfPossible(Direction::UP) && _gameState == STATE_PLAYING) {
					_prevScore = _score;
					_grid.execute(Direction::UP);

					sf::Clock clock;
					for (int i = 0; i < 10; i++) {
						clock.restart();
						_grid.animate(10);
						this->update(1.0f / 60.0f);
						this->draw(1.0f / 60.0f);
						float time = clock.getElapsedTime().asMilliseconds();
						if (time < (100 / 10)) {
							Sleep((100 - time) / 10);
						}
					}

					_grid.moveToTiles();

					this->updateScore();
					this->updateHighscore();
					this->updateFiles();
					this->checkIfOver();
				}

				// Keyboard down
				if (sf::Keyboard::Down == event.key.code && _grid.checkIfPossible(Direction::DOWN) && _gameState == STATE_PLAYING) {
					_prevScore = _score;
					_grid.execute(Direction::DOWN);
					
					sf::Clock clock;
					for (int i = 0; i < 10; i++) {
						clock.restart();
						_grid.animate(10);
						this->update(1.0f / 60.0f);
						this->draw(1.0f / 60.0f);
						float time = clock.getElapsedTime().asMilliseconds();
						if (time < (100 / 10)) {
							Sleep((100 - time) / 10);
						}
					}

					_grid.moveToTiles();

					this->updateScore();
					this->updateHighscore();
					this->updateFiles();
					this->checkIfOver();
				}
				break;
			}
		}
	}

	void GameState::update(float frameDif) {
		// Fade in
		if (this->_fadeClock.getElapsedTime().asSeconds() > 0.01 && this->_opacity > 0 && this->_clock.getElapsedTime().asSeconds() < 1) {
			this->_opacity -= 5;
			_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));
			this->_fadeClock.restart();
		}

		// Play music
		if (this->_clock.getElapsedTime().asSeconds() > 0.5 && this->_clock.getElapsedTime().asSeconds() < 2) {
			if (this->_data->assets.getSoundStatus("Game Music") == sf::Music::Playing) {}
			else this->_data->assets.playSound("Game Music", true);
		}


		// Button animations

		if (this->_data->input.isMouseOver(_pauseButton, this->_data->window)) {
			if (this->_data->input.isSpriteClicked(_pauseButton, sf::Mouse::Left, this->_data->window)) {
				this->_pauseButton.setScale(_pauseScale, _pauseScale);
			}
			else if (this->_pauseButton.getScale().x < 1.2f * _pauseScale) {
				this->_pauseButton.scale(1.05f, 1.05f);
			}
		}

		if (!this->_data->input.isMouseOver(_pauseButton, this->_data->window)) {
			if (this->_pauseButton.getScale().x > _pauseScale) {
				this->_pauseButton.scale(0.95f, 0.95f);
			}
		}

		if (this->_data->input.isMouseOver(_prevButton, this->_data->window)) {
			if (this->_data->input.isSpriteClicked(_prevButton, sf::Mouse::Left, this->_data->window)) {
				this->_prevButton.setScale(_prevScale, _prevScale);
			}
			else if (this->_prevButton.getScale().x < 1.2f * _prevScale) {
				this->_prevButton.scale(1.05f, 1.05f);
			}
		}

		if (!this->_data->input.isMouseOver(_prevButton, this->_data->window)) {
			if (this->_prevButton.getScale().x > _prevScale) {
				this->_prevButton.scale(0.95f, 0.95f);
			}
		}

		if (_gameState == STATE_PAUSED) {
			_overBox.update();
		}

		if (_gameState == STATE_RESTART) {
			_gameState = STATE_PLAYING;
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
					// Reset files
					{
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
					this->_data->machine.addState(StatePtr(new GameState(_data)), true);
				}
			}
		}

		if (_gameState == STATE_HOME) {
			_gameState = STATE_PLAYING;
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

	void GameState::resume() {
		// Reset clocks
		this->_clock.restart();
		this->_fadeClock.restart();

		// Fade in
		if (this->_fadeClock.getElapsedTime().asSeconds() > 0.01 && this->_opacity > 0 && this->_clock.getElapsedTime().asSeconds() < 1) {
			this->_opacity -= 5;
			_fade.setFillColor(sf::Color(0, 0, 0, this->_opacity));
			this->_fadeClock.restart();
		}
	}

	void GameState::draw(float frameDif) {
		this->_data->window.clear(sf::Color(20, 66, 99));

		this->_data->window.draw(this->_pauseButton);
		this->_data->window.draw(this->_prevButton);
		this->_data->window.draw(this->_title);
		this->_grid.draw(this->_data->window);
		this->_scoreBoard.draw(this->_data->window);
		this->_highscoreBoard.draw(this->_data->window);
		this->_data->window.draw(this->_fade);

		if (_gameState == STATE_PAUSED) {
			_overBox.draw(this->_data->window);
		}

		this->_data->window.display();
	}

	void GameState::updateScore() {
		_score = _grid.score;
		_scoreBoard.changeText(std::to_string(_grid.score));
	}

	void GameState::updateHighscore() {
		if (_score > _highscore) {
			_highscore = _score;
			_highscoreBoard.changeText(std::to_string(_highscore));
		}
	}

	void GameState::updateFiles() {
		std::vector<int> output;

		// Update score
		std::vector<int> scores;
		scores.push_back(_score);
		scores.push_back(_prevScore);

		this->_data->files.writeFile("res/data/Score.txt", scores);

		// Update highscore
		this->_data->files.writeFile("res/data/Highscore.txt", _highscore);

		// Update array
		std::vector<int> vector;
		std::vector<std::vector<int>> array = _grid.getArray();
		for (int y = 0; y < _numberOfTiles; y++) {
			for (int x = 0; x < _numberOfTiles; x++) {
				vector.push_back(array[y][x]);
			}
		}

		this->_data->files.writeFile("res/data/Array.txt", vector);

		// Update prev
		vector.clear();
		array.clear();
		array = _grid.getPrev();
		for (int y = 0; y < _numberOfTiles; y++) {
			for (int x = 0; x < _numberOfTiles; x++) {
				vector.push_back(array[y][x]);
			}
		}

		this->_data->files.writeFile("res/data/Prev.txt", vector);
	}

	void GameState::checkIfOver() {
		if (_grid.isThere2048()) {
			_overBox.init(_data, "Win Box");
			_gameState = STATE_PAUSED;
		}

		else if (!_grid.checkIfPossible(Direction::LEFT) && !_grid.checkIfPossible(Direction::RIGHT) && !_grid.checkIfPossible(Direction::UP) && !_grid.checkIfPossible(Direction::DOWN)) {
			_overBox.init(_data, "Lose Box");
			_gameState = STATE_PAUSED;
		}
	}
}