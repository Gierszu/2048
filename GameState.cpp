#include "GameState.h"
#include "PauseState.h"
#include "DEFINITIONS.h"
//#include "GameOverState.h"

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
		_score = this->_data->files.readSingleFile("res/data/Score.txt");

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
				Sleep(100 / 20);
			}
		}
		_grid.addToScore(_score);

		_grid.moveToTiles();
		_grid.setTiles();

	}

	void GameState::handleInput() {
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
					// Prev button released
					if (this->_data->input.isMouseOver(this->_prevButton, this->_data->window)) {
						_grid.setArrToPrev();
						_grid.moveToTiles();
						_grid.setTiles();
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
				if (sf::Keyboard::Left == event.key.code && _grid.checkIfPossible(Direction::LEFT)) {
					_grid.execute(Direction::LEFT);

					for (int i = 0; i < 20; i++) {
						_grid.animate(20);
						this->update(1.0f / 60.0f);
						this->draw(1.0f / 60.0f);
						Sleep(100 / 20);
					}

					_grid.moveToTiles();

					this->updateScore();
					this->updateHighscore();
					this->updateFiles();
				}

				// Keyboard right
				if (sf::Keyboard::Right == event.key.code && _grid.checkIfPossible(Direction::RIGHT)) {
					_grid.execute(Direction::RIGHT);

					for (int i = 0; i < 20; i++) {
						_grid.animate(20);
						this->update(1.0f / 60.0f);
						this->draw(1.0f / 60.0f);
						Sleep(100 / 20);
					}

					_grid.moveToTiles();

					this->updateScore();
					this->updateHighscore();
					this->updateFiles();
				}

				// Keyboard up
				if (sf::Keyboard::Up == event.key.code && _grid.checkIfPossible(Direction::UP)) {
					_grid.execute(Direction::UP);

					for (int i = 0; i < 20; i++) {
						_grid.animate(20);
						this->update(1.0f / 60.0f);
						this->draw(1.0f / 60.0f);
						Sleep(100 / 20);
					}

					_grid.moveToTiles();

					this->updateScore();
					this->updateHighscore();
					this->updateFiles();
				}

				// Keyboard down
				if (sf::Keyboard::Down == event.key.code && _grid.checkIfPossible(Direction::DOWN)) {
					_grid.execute(Direction::DOWN);

					for (int i = 0; i < 20; i++) {
						_grid.animate(20);
						this->update(1.0f / 60.0f);
						this->draw(1.0f / 60.0f);
						Sleep(100 / 20);
					}

					_grid.moveToTiles();

					this->updateScore();
					this->updateHighscore();
					this->updateFiles();
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
		this->_data->files.writeFile("res/data/Score.txt", _score);

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
		array = _grid.getPrev();
		for (int y = 0; y < _numberOfTiles; y++) {
			for (int x = 0; x < _numberOfTiles; x++) {
				vector.push_back(array[y][x]);
			}
		}

		this->_data->files.writeFile("res/data/Prev.txt", vector);
	}

	/*void GameState::initGridBackTiles() {
		sf::Vector2f size;
		size.x = this->_gridBack.getGlobalBounds().width / 4.0f;
		size.y = this->_gridBack.getGlobalBounds().height / 4.0f;

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				_gridBackTiles[x][y] = _gridZeroSprite;
				_gridBackTiles[x][y].setPosition((_gridBack.getPosition().x + (size.x * x) - size.x * 1.5f), (_gridBack.getPosition().y + size.y * y) - size.y * 1.5f);
				_gridArray[x][y] = 0;

				//Grid tiles
				_gridTiles[x][y] = _gridZeroSprite;
				_gridTiles[x][y].setPosition((_gridBack.getPosition().x + (size.x * x) - size.x * 1.5f), (_gridBack.getPosition().y + size.y * y) - size.y * 1.5f);
			}
		}
	}*/

	/*void GameState::moveArray(Direction dir, int array[4][4], int prev[4][4]) {

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				prev[x][y] = array[x][y];
			}
		}

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				_vectors[x][y].x = 0;
				_vectors[x][y].y = 0;
			}
		}

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				origin[x][y] = 0;
			}
		}

		switch (dir) {
		case Direction::LEFT:
			for (int x = 0; x < 4; x++) {
				for (int y = 0; y < 4; y++) {
					if (x < 0 || y < 0 || x > 3 || y > 3) {}
					else {
						if (x - 3 >= 0 && array[x - 3][y] == 0 && array[x][y] != 0) {
							array[x - 3][y] = array[x][y];
							array[x][y] = 0;
							_vectors[x][y].x = -3;
							_vectors[x][y].y = 0;
							origin[x - 3][y] = 3;
						}
						else if (x - 2 >= 0 && array[x - 2][y] == 0 && array[x][y] != 0) {
							array[x - 2][y] = array[x][y];
							array[x][y] = 0;
							_vectors[x][y].x = -2;
							_vectors[x][y].y = 0;
							origin[x - 2][y] = 2;
						}
						else if (x - 1 >= 0 && array[x - 1][y] == 0 && array[x][y] != 0) {
							array[x - 1][y] = array[x][y];
							array[x][y] = 0;
							_vectors[x][y].x = -1;
							_vectors[x][y].y = 0;
							origin[x - 1][y] = 1;
						}
						else {
							_vectors[x][y].x = 0;
							_vectors[x][y].y = 0;
							origin[x][y] = 0;
						}
					}
				}
			}
			for (int x = 0; x < 4; x++) {
				for (int y = 0; y < 4; y++) {
					if (x < 0 || y < 0 || x > 3 || y > 3) {}
					else {
						if (x + 1 < 4 && array[x + 1][y] == array[x][y] && array[x][y] != 0) {
							array[x][y] += array[x + 1][y];
							array[x + 1][y] = 0;
							_vectors[x + 1 + origin[x + 1][y]][y].x += -1;
							origin[x + 1 + origin[x + 1][y]][y] += 1;
							if (x + 2 < 4 && array[x + 2][y] != 0) {
								array[x + 1][y] = array[x + 2][y];
								array[x + 2][y] = 0;
								_vectors[x + 2 + origin[x + 2][y]][y].x += -1;
								origin[x + 2 + origin[x + 2][y]][y] += 1;
							}
							if (x + 3 < 4 && array[x + 3][y] != 0) {
								array[x + 2][y] = array[x + 3][y];
								array[x + 3][y] = 0;
								_vectors[x + 3 + origin[x + 3][y]][y].x += -1;
								origin[x + 3 + origin[x + 3][y]][y] += 1;
							}
						}
					}
				}
			}
			break;
		case Direction::RIGHT:
			for (int x = 3; x >= 0; x--) {
				for (int y = 0; y < 4; y++) {
					if (x < 0 || y < 0 || x > 3 || y > 3) {}
					else {
						if (x + 3 < 4 && array[x + 3][y] == 0 && array[x][y] != 0) {
							array[x + 3][y] = array[x][y];
							array[x][y] = 0;
							_vectors[x][y].x = 3;
							_vectors[x][y].y = 0;
							origin[x + 3][y] = -3;
						}
						else if (x + 2 < 4 && array[x + 2][y] == 0 && array[x][y] != 0) {
							array[x + 2][y] = array[x][y];
							array[x][y] = 0;
							_vectors[x][y].x = 2;
							_vectors[x][y].y = 0;
							origin[x + 2][y] = -2;
						}
						else if (x + 1 < 4 && array[x + 1][y] == 0 && array[x][y] != 0) {
							array[x + 1][y] = array[x][y];
							array[x][y] = 0;
							_vectors[x][y].x = 1;
							_vectors[x][y].y = 0;
							origin[x + 1][y] = -1;
						}
						else {
							_vectors[x][y].x = 0;
							_vectors[x][y].y = 0;
						}
					}
				}
			}
			for (int x = 3; x >= 0; x--) {
				for (int y = 0; y < 4; y++) {
					if (x < 0 || y < 0 || x > 3 || y > 3) {}
					else {
						if (x - 1 >= 0 && array[x - 1][y] == array[x][y] && array[x][y] != 0) {
							array[x][y] += array[x - 1][y];
							array[x - 1][y] = 0;
							_vectors[x - 1 + origin[x - 1][y]][y].x += 1;
							origin[x - 1 + origin[x - 1][y]][y] += -1;
							if (x - 2 >= 0 && array[x - 2][y] != 0) {
								array[x - 1][y] = array[x - 2][y];
								array[x - 2][y] = 0;
								_vectors[x - 2 + origin[x - 2][y]][y].x += 1;
								origin[x - 2 + origin[x - 2][y]][y] += -1;
							}
							if (x - 3 >= 0 && array[x - 3][y] != 0) {
								array[x - 2][y] = array[x - 3][y];
								array[x - 3][y] = 0;
								_vectors[x - 3][y].x += 1;
								origin[x - 3][y] += -1;
							}
							if (array[x][y] == 0) {
								_vectors[x][y].x = 0;
								_vectors[x][y].y = 0;
							}
						}
					}
				}
			}
			break;
		case Direction::UP:
			for (int y = 0; y < 4; y++) {
				for (int x = 0; x < 4; x++) {
					if (y < 0 || x < 0 || y > 3 || x > 3) {}
					else {
						if (y - 3 >= 0 && array[x][y - 3] == 0 && array[x][y] != 0) {
							array[x][y - 3] = array[x][y];
							array[x][y] = 0;
							_vectors[x][y].x = 0;
							_vectors[x][y].y = -3;
							origin[x][y - 3] = 3;
						}
						else if (y - 2 >= 0 && array[x][y - 2] == 0 && array[x][y] != 0) {
							array[x][y - 2] = array[x][y];
							array[x][y] = 0;
							_vectors[x][y].x = 0;
							_vectors[x][y].y = -2;
							origin[x][y - 2] = 2;
						}
						else if (y - 1 >= 0 && array[x][y - 1] == 0 && array[x][y] != 0) {
							array[x][y - 1] = array[x][y];
							array[x][y] = 0;
							_vectors[x][y].x = 0;
							_vectors[x][y].y = -1;
							origin[x][y - 1] = 1;
						}
						else {
							_vectors[x][y].x = 0;
							_vectors[x][y].y = 0;
						}
					}
				}
			}
			for (int y = 0; y < 4; y++) {
				for (int x = 0; x < 4; x++) {
					if (x < 0 || y < 0 || x > 3 || y > 3) {}
					else {
						if (y + 1 < 4 && array[x][y + 1] == array[x][y] && array[x][y] != 0) {
							array[x][y] += array[x][y + 1];
							array[x][y + 1] = 0;
							_vectors[x][y + 1 + origin[x][y + 1]].y += -1;
							origin[x][y + 1 + origin[x][y + 1]] += 1;
							if (y + 2 < 4 && array[x][y + 2] != 0) {
								array[x][y + 1] = array[x][y + 2];
								array[x][y + 2] = 0;
								_vectors[x][y + 2 + origin[x][y + 2]].y += -1;
								origin[x][y + 2 + origin[x][y + 2]] += 1;
							}
							if (y + 3 < 4 && array[x][y + 3] != 0) {
								array[x][y + 2] = array[x][y + 3];
								array[x][y + 3] = 0;
								_vectors[x][y + 3 + origin[x][y + 3]].y += -1;
								origin[x][y + 3 + origin[x][y + 3]] += 1;
							}
						}
					}
				}
			}
			break;
		case Direction::DOWN:
			for (int y = 3; y >= 0; y--) {
				for (int x = 0; x < 4; x++) {
					if (y < 0 || x < 0 || y > 3 || x > 3) {}
					else {
						if (y + 3 < 4 && array[x][y + 3] == 0 && array[x][y] != 0) {
							array[x][y + 3] = array[x][y];
							array[x][y] = 0;
							_vectors[x][y].x = 0;
							_vectors[x][y].y = 3;
							origin[x][y + 3] = -3;
						}
						else if (y + 2 < 4 && array[x][y + 2] == 0 && array[x][y] != 0) {
							array[x][y + 2] = array[x][y];
							array[x][y] = 0;
							_vectors[x][y].x = 0;
							_vectors[x][y].y = 2;
							origin[x][y + 2] = -2;
						}
						else if (y + 1 < 4 && array[x][y + 1] == 0 && array[x][y] != 0) {
							array[x][y + 1] = array[x][y];
							array[x][y] = 0;
							_vectors[x][y].x = 0;
							_vectors[x][y].y = 1;
							origin[x][y + 1] = -1;
						}
						else {
							_vectors[x][y].x = 0;
							_vectors[x][y].y = 0;
						}
					}
				}
			}
			for (int y = 3; y >= 0; y--) {
				for (int x = 0; x < 4; x++) {
					if (x < 0 || y < 0 || x > 3 || y > 3) {}
					else {
						if (y - 1 >= 0 && array[x][y - 1] == array[x][y] && array[x][y] != 0) {
							array[x][y] += array[x][y - 1];
							array[x][y - 1] = 0;
							_vectors[x][y - 1 + origin[x][y - 1]].y += 1;
							origin[x][y - 1 + origin[x][y - 1]] += -1;
							if (y - 2 >= 0 && array[x][y - 2] != 0) {
								array[x][y - 1] = array[x][y - 2];
								array[x][y - 2] = 0;
								_vectors[x][y - 2 + origin[x][y - 2]].y += 1;
								origin[x][y - 2 + origin[x][y - 2]] += -1;
							}
							if (y - 3 >= 0 && array[x][y - 3] != 0) {
								array[x][y - 2] = array[x][y - 3];
								array[x][y - 3] = 0;
								_vectors[x][y - 3].y += 1;
								origin[x][y - 3] += -1;
							}
							if (array[x][y] == 0) {
								_vectors[x][y].x = 0;
								_vectors[x][y].y = 0;
							}
						}
					}
				}
			}
			break;
		}
	}*/

	/*void GameState::moveTiles() {
		float time = 150;
		int times = 10;
		int counter = times;
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				std::cout << "| " << _vectors[x][y].x << " : " << _vectors[x][y].y << " |";
			}
			std::cout << std::endl;
		}
		
		while (counter) {
			for (int x = 0; x < 4; x++) {
				for (int y = 0; y < 4; y++) {
					float disX = _vectors[x][y].x * this->_gridZeroSprite.getGlobalBounds().width / times;
					float disY = _vectors[x][y].y * this->_gridZeroSprite.getGlobalBounds().height / times;

					this->_gridTiles[x][y].move(sf::Vector2f(disX, disY));
				}
			}
			this->update(1.0f / 60.0f);
			this->draw(1.0f / 60.0f);
			Sleep(time/times);
			counter--;
		}
		std::cout << std::endl;
	}*/

	/*void GameState::setTiles() {
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				switch (_gridArray[x][y]) {
				case 0:
					this->_gridTiles[x][y].setTexture(this->_data->assets.getTexture("0"));
					this->_gridTiles[x][y].setColor(sf::Color(255, 255, 255, 0));
					break;
				case 2:
					this->_gridTiles[x][y].setTexture(this->_data->assets.getTexture("2"));
					this->_gridTiles[x][y].setColor(sf::Color(255, 255, 255, 255));
					break;
				case 4:
					this->_gridTiles[x][y].setTexture(this->_data->assets.getTexture("4"));
					this->_gridTiles[x][y].setColor(sf::Color(255, 255, 255, 255));
					break;
				case 8:
					this->_gridTiles[x][y].setTexture(this->_data->assets.getTexture("8"));
					this->_gridTiles[x][y].setColor(sf::Color(255, 255, 255, 255));
					break;
				case 16:
					this->_gridTiles[x][y].setTexture(this->_data->assets.getTexture("16"));
					this->_gridTiles[x][y].setColor(sf::Color(255, 255, 255, 255));
					break;
				case 32:
					this->_gridTiles[x][y].setTexture(this->_data->assets.getTexture("32"));
					this->_gridTiles[x][y].setColor(sf::Color(255, 255, 255, 255));
					break;
				case 64:
					this->_gridTiles[x][y].setTexture(this->_data->assets.getTexture("64"));
					this->_gridTiles[x][y].setColor(sf::Color(255, 255, 255, 255));
					break;
				case 128:
					this->_gridTiles[x][y].setTexture(this->_data->assets.getTexture("128"));
					this->_gridTiles[x][y].setColor(sf::Color(255, 255, 255, 255));
					break;
				case 256:
					this->_gridTiles[x][y].setTexture(this->_data->assets.getTexture("256"));
					this->_gridTiles[x][y].setColor(sf::Color(255, 255, 255, 255));
					break;
				case 512:
					this->_gridTiles[x][y].setTexture(this->_data->assets.getTexture("512"));
					this->_gridTiles[x][y].setColor(sf::Color(255, 255, 255, 255));
					break;
				case 1024:
					this->_gridTiles[x][y].setTexture(this->_data->assets.getTexture("1024"));
					this->_gridTiles[x][y].setColor(sf::Color(255, 255, 255, 255));
					break;
				case 2048:
					this->_gridTiles[x][y].setTexture(this->_data->assets.getTexture("2048"));
					this->_gridTiles[x][y].setColor(sf::Color(255, 255, 255, 255));
					break;
				}

				this->_gridTiles[x][y].setPosition(sf::Vector2f(this->_gridBackTiles[x][y].getPosition().x, this->_gridBackTiles[x][y].getPosition().y));
			}
		}
	}*/

	/*void GameState::checkPlayerHasWon() {
		
	}*/

	/*void GameState::spawnTile() {
		std::default_random_engine RNG(time(NULL));
		std::uniform_int_distribution<int> tileR(0, 3);
		std::uniform_real_distribution<float> chance(0, 1);

		int X = tileR(RNG);
		int Y = tileR(RNG);

		while (!_gridArray[X][Y] == 0) {
			X = tileR(RNG);
			Y = tileR(RNG);
		}
		if (chance(RNG) > 0.05) { 
			this->_gridArray[X][Y] = 2;
		}
		else { 
			this->_gridArray[X][Y] = 4;
		}
	}*/

	/*bool GameState::checkIfPossible(Direction dir) {
		int array[4][4], prev[4][4];

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				array[x][y] = _gridArray[x][y];
				prev[x][y] = _prevArray[x][y];
			}
		}

		moveArray(dir, array, prev);

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				if (array[x][y] != prev[x][y]) {
					return true;
				}
			}
		}
		return false;
	}*/
}