#include <sstream>
#include <iostream>
#include <random>
#include <ctime>

#include "Grid.h"
#include <Windows.h>


namespace game {

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////													Grid													////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void Grid::init(GameDataPtr data, sf::Vector2f pos, float size, int number) {
		this->_data = data;
		this->_gridSize = 0.99f * size;
		this->_tileSize = this->_gridSize / number;
		this->_numberOfTiles = number;

		// Set textures	
		this->_body.setTexture(this->_data->assets.getTexture("Back"));
		this->_zero.setTexture(this->_data->assets.getTexture("0"));

		// Set origins
		this->_body.setOrigin(sf::Vector2f(this->_body.getGlobalBounds().width / 2.0f, this->_body.getGlobalBounds().height / 2.0f));
		this->_zero.setOrigin(sf::Vector2f(this->_zero.getGlobalBounds().width / 2.0f, this->_zero.getGlobalBounds().height / 2.0f));

		// Scale sprites
		this->_body.setScale(sf::Vector2f(size / (_body.getGlobalBounds().height), size / (_body.getGlobalBounds().height)));
		this->_zero.setScale(sf::Vector2f((this->_tileSize - (0.06f * this->_tileSize)) / (_zero.getGlobalBounds().height), (this->_tileSize - (0.06f * this->_tileSize)) / (_zero.getGlobalBounds().height)));

		// Fill back tiles, fill array with zeros
		for (int y = 0; y < number; y++) {
			std::vector<int> intvector;
			std::vector<sf::Sprite> vector;
			for (int x = 0; x < number; x++) {
				vector.push_back(this->_zero);
				intvector.push_back(0);
			}
			this->_gridArray.push_back(intvector);
			this->_gridZero.push_back(vector);
		}

		// Set positions
		this->_body.setPosition(pos);
		for (int y = 0; y < number; y++) {
			for (int x = 0; x < number; x++) {
				this->_gridZero[y][x].setPosition((this->_body.getPosition().x + (this->_tileSize * x) - this->_tileSize * 1.5f), (this->_body.getPosition().y + this->_tileSize * y) - this->_tileSize * 1.5f);
			}
		}
	}

	void Grid::init(GameDataPtr data, sf::Vector2f pos, float size, int number, std::vector<std::vector<int>> array, std::vector<std::vector<int>> prev) {
		this->_data = data;
		this->_gridSize = 0.99f * size;
		this->_tileSize = this->_gridSize / number;
		this->_numberOfTiles = number;
		this->_gridArray = array;
		this->_prevArray = array;

		// Set textures	
		this->_body.setTexture(this->_data->assets.getTexture("Back"));
		this->_zero.setTexture(this->_data->assets.getTexture("0"));

		// Set origins
		this->_body.setOrigin(sf::Vector2f(this->_body.getGlobalBounds().width / 2.0f, this->_body.getGlobalBounds().height / 2.0f));
		this->_zero.setOrigin(sf::Vector2f(this->_zero.getGlobalBounds().width / 2.0f, this->_zero.getGlobalBounds().height / 2.0f));

		// Scale sprites
		this->_body.setScale(sf::Vector2f(size / (_body.getGlobalBounds().height), size / (_body.getGlobalBounds().height)));
		this->_zero.setScale(sf::Vector2f((this->_tileSize - (0.06f * this->_tileSize)) / (_zero.getGlobalBounds().height), (this->_tileSize - (0.06f * this->_tileSize)) / (_zero.getGlobalBounds().height)));

		// Fill back tiles
		for (int y = 0; y < number; y++) {
			std::vector<sf::Sprite> vector;
			for (int x = 0; x < number; x++) {
				vector.push_back(this->_zero);
			}
			this->_gridZero.push_back(vector);
		}

		// Set positions
		this->_body.setPosition(pos);
		for (int y = 0; y < number; y++) {
			for (int x = 0; x < number; x++) {
				this->_gridZero[y][x].setPosition((this->_body.getPosition().x + (this->_tileSize * x) - this->_tileSize * 1.5f), (this->_body.getPosition().y + this->_tileSize * y) - this->_tileSize * 1.5f);
			}
		}
	}

	void Grid::initTiles() {
		this->spawnTile();
		this->spawnTile();
		this->setPrevToArr();
	}

	void Grid::setPrevToArr() {
		_prevArray = _gridArray;
	}

	void Grid::setArrToPrev() {
		_gridArray = _prevArray;
	}

	void Grid::draw(sf::RenderWindow& window) {
		window.draw(this->_body);
		for (int y = 0; y < this->_numberOfTiles; y++) {
			for (int x = 0; x < this->_numberOfTiles; x++) {
				window.draw(this->_gridZero[y][x]);
			}
		}
		for (int i = 0; i < _spawnedTiles.size(); i++) {
			_spawnedTiles[i].draw(this->_data->window);
		}
		for (int i = 0; i < _poppedTiles.size(); i++) {
			_poppedTiles[i].draw(this->_data->window);
		}
		for (int i = 0; i < _tiles.size(); i++) {
			_tiles[i].draw(this->_data->window);
		}
	}

	bool Grid::checkIfPossible(Direction dir) {
		std::vector<Tile> tiles = _tiles;
		std::vector<std::vector<int>> array = _gridArray;
		std::vector<std::vector<int>> prev = _prevArray;

		this->setMove(dir, array, prev, tiles);

		for (int y = 0; y < _numberOfTiles; y++) {
			for (int x = 0; x < _numberOfTiles; x++) {
				if (array[y][x] != prev[y][x]) {
					return true;
				}
			}
		}
		return false;
	}

	void Grid::setMove(Direction dir, std::vector<std::vector<int>>& array, std::vector<std::vector<int>>& prev, std::vector<Tile>& tiles) {
		prev = array;

		switch (dir) {
		case Direction::LEFT:
			for (int x = 0; x < _numberOfTiles; x++) {
				for (int y = 0; y < _numberOfTiles; y++) {
					for (int i = _numberOfTiles - 1; i >= 0; i--) {
						if (x - i >= 0 && array[y][x - i] == 0 && array[y][x] != 0) {
							array[y][x - i] = array[y][x];
							array[y][x] = 0;
							for (int j = 0; j < tiles.size(); j++) {
								if (tiles[j].newPos == sf::Vector2i(x, y)) {
									tiles[j].newPos.x -= i;
								}
							}
						}
					}
				}
			}
			for (int x = 0; x < _numberOfTiles; x++) {
				for (int y = 0; y < _numberOfTiles; y++) {
					if (x + 1 < _numberOfTiles && array[y][x + 1] == array[y][x] && array[y][x] != 0) {
						array[y][x] += array[y][x + 1];
						addToScore(array[y][x]);
						array[y][x + 1] = 0;
						for (int j = 0; j < tiles.size(); j++) {
							if (tiles[j].newPos == sf::Vector2i(x + 1, y)) {
								tiles[j].newPos.x -= 1;
							}
						}
					}
				}
			}
			for (int x = 0; x < _numberOfTiles; x++) {
				for (int y = 0; y < _numberOfTiles; y++) {
					for (int i = _numberOfTiles - 1; i >= 0; i--) {
						if (x - i >= 0 && array[y][x - i] == 0 && array[y][x] != 0) {
							array[y][x - i] = array[y][x];
							array[y][x] = 0;
							for (int j = 0; j < tiles.size(); j++) {
								if (tiles[j].newPos == sf::Vector2i(x, y)) {
									tiles[j].newPos.x -= i;
								}
							}
						}
					}
				}
			}
			break;
		case Direction::RIGHT:
			for (int x = _numberOfTiles - 1; x >= 0; x--) {
				for (int y = 0; y < _numberOfTiles; y++) {
					for (int i = _numberOfTiles - 1; i >= 0; i--) {
						if (x + i < _numberOfTiles && array[y][x + i] == 0 && array[y][x] != 0) {
							array[y][x + i] = array[y][x];
							array[y][x] = 0;
							for (int j = 0; j < tiles.size(); j++) {
								if (tiles[j].newPos == sf::Vector2i(x, y)) {
									tiles[j].newPos.x += i;
								}
							}
						}
					}
				}
			}
			for (int x = _numberOfTiles - 1; x >= 0; x--) {
				for (int y = 0; y < _numberOfTiles; y++) {
					if (x - 1 >= 0 && array[y][x - 1] == array[y][x] && array[y][x] != 0) {
						array[y][x] += array[y][x - 1];
						addToScore(array[y][x]);
						array[y][x - 1] = 0;
						for (int j = 0; j < tiles.size(); j++) {
							if (tiles[j].newPos == sf::Vector2i(x - 1, y)) {
								tiles[j].newPos.x += 1;
							}
						}
					}
				}
			}
			for (int x = _numberOfTiles - 1; x >= 0; x--) {
				for (int y = 0; y < _numberOfTiles; y++) {
					for (int i = _numberOfTiles - 1; i >= 0; i--) {
						if (x + i < _numberOfTiles && array[y][x + i] == 0 && array[y][x] != 0) {
							array[y][x + i] = array[y][x];
							array[y][x] = 0;
							for (int j = 0; j < tiles.size(); j++) {
								if (tiles[j].newPos == sf::Vector2i(x, y)) {
									tiles[j].newPos.x += i;
								}
							}
						}
					}
				}
			}
			break;
		case Direction::UP:
			for (int y = 0; y < _numberOfTiles; y++) {
				for (int x = 0; x < _numberOfTiles; x++) {
					for (int i = _numberOfTiles - 1; i >= 0; i--) {
						if (y - i >= 0 && array[y - i][x] == 0 && array[y][x] != 0) {
							array[y - i][x] = array[y][x];
							array[y][x] = 0;
							for (int j = 0; j < tiles.size(); j++) {
								if (tiles[j].newPos == sf::Vector2i(x, y)) {
									tiles[j].newPos.y -= i;
								}
							}
						}
					}
				}
			}
			for (int y = 0; y < _numberOfTiles; y++) {
				for (int x = 0; x < _numberOfTiles; x++) {
					if (y + 1 < _numberOfTiles && array[y + 1][x] == array[y][x] && array[y][x] != 0) {
						array[y][x] += array[y + 1][x];
						addToScore(array[y][x]);
						array[y + 1][x] = 0;
						for (int j = 0; j < tiles.size(); j++) {
							if (tiles[j].newPos == sf::Vector2i(x, y + 1)) {
								tiles[j].newPos.y -= 1;
							}
						}
					}
				}
			}
			for (int y = 0; y < _numberOfTiles; y++) {
				for (int x = 0; x < _numberOfTiles; x++) {
					for (int i = _numberOfTiles - 1; i >= 0; i--) {
						if (y - i >= 0 && array[y - i][x] == 0 && array[y][x] != 0) {
							array[y - i][x] = array[y][x];
							array[y][x] = 0;
							for (int j = 0; j < tiles.size(); j++) {
								if (tiles[j].newPos == sf::Vector2i(x, y)) {
									tiles[j].newPos.y -= i;
								}
							}
						}
					}
				}
			}
			break;
		case Direction::DOWN:
			for (int y = _numberOfTiles - 1; y >= 0; y--) {
				for (int x = 0; x < _numberOfTiles; x++) {
					for (int i = _numberOfTiles - 1; i >= 0; i--) {
						if (y + i < _numberOfTiles && array[y + i][x] == 0 && array[y][x] != 0) {
							array[y + i][x] = array[y][x];
							array[y][x] = 0;
							for (int j = 0; j < tiles.size(); j++) {
								if (tiles[j].newPos == sf::Vector2i(x, y)) {
									tiles[j].newPos.y += i;
								}
							}
						}
					}
				}
			}
			for (int y = _numberOfTiles - 1; y >= 0; y--) {
				for (int x = 0; x < _numberOfTiles; x++) {
					if (y - 1 >= 0 && array[y - 1][x] == array[y][x] && array[y][x] != 0) {
						array[y][x] += array[y - 1][x];
						addToScore(array[y][x]);
						array[y - 1][x] = 0;
						for (int j = 0; j < tiles.size(); j++) {
							if (tiles[j].newPos == sf::Vector2i(x, y - 1)) {
								tiles[j].newPos.y += 1;
							}
						}
					}
				}
			}
			for (int y = _numberOfTiles - 1; y >= 0; y--) {
				for (int x = 0; x < _numberOfTiles; x++) {
					for (int i = _numberOfTiles - 1; i >= 0; i--) {
						if (y + i < _numberOfTiles && array[y + i][x] == 0 && array[y][x] != 0) {
							array[y + i][x] = array[y][x];
							array[y][x] = 0;
							for (int j = 0; j < tiles.size(); j++) {
								if (tiles[j].newPos == sf::Vector2i(x, y)) {
									tiles[j].newPos.y += i;
								}
							}
						}
					}
				}
			}
			break;
		}
	}

	void Grid::spawnTile() {
		std::default_random_engine RNG(time(NULL));
		std::uniform_int_distribution<int> tileR(0, 3);
		std::uniform_real_distribution<float> chance(0, 1);

		int X = tileR(RNG);
		int Y = tileR(RNG);

		while (!_gridArray[Y][X] == 0) {
			X = tileR(RNG);
			Y = tileR(RNG);
		}
		if (chance(RNG) > 0.05) {
			this->_gridArray[Y][X] = 2;
		}
		else {
			this->_gridArray[Y][X] = 4;
		}
		Tile tile;
		tile.init(_data, _gridArray[Y][X], sf::Vector2i(X, Y), sf::Vector2f(_gridZero[Y][X].getPosition().x, _gridZero[Y][X].getPosition().y), _tileSize);
		tile.setScale(0.0f);
		_spawnedTiles.push_back(tile);
	}

	void Grid::setTiles() {

		this->_tiles.clear();

		for (int y = 0; y < this->_numberOfTiles; y++) {
			for (int x = 0; x < this->_numberOfTiles; x++) {
				switch (this->_gridArray[y][x]) {
				case 0:
					break;
				default:
					Tile tile;
					tile.init(_data, _gridArray[y][x], sf::Vector2i(x, y), sf::Vector2f(_gridZero[y][x].getPosition().x, _gridZero[y][x].getPosition().y), _tileSize);
					_tiles.push_back(tile);
					break;
				}
			}
		}
	}

	void Grid::animate(int div) {
		std::vector<std::thread> threads;

		// Spawn tiles
		for (std::vector<Tile>::iterator it = _spawnedTiles.begin(); it != _spawnedTiles.end(); ++it) {
			Tile* tilePtr = &(*it);
			threads.emplace_back(std::thread(&Tile::spawnTile, tilePtr, div));
		}

		// Move tiles
		for (std::vector<Tile>::iterator itt = _tiles.begin(); itt != _tiles.end(); ++itt) {
			Tile* tilePtr = &(*itt);
			threads.emplace_back(std::thread(&Tile::move, tilePtr, div));
		}

		// Wait for threads
		for (int i = 0; i < threads.size(); i++) {
			threads[i].join();
		}
	}

	void Grid::moveToTiles() {
		for (int i = 0; i < _spawnedTiles.size(); i++) {
			_tiles.push_back(_spawnedTiles[i]);
		}
		for (int i = 0; i < _poppedTiles.size(); i++) {
			_tiles.push_back(_poppedTiles[i]);
		}
		_spawnedTiles.clear();
		_poppedTiles.clear();

		setTiles();
	}

	void Grid::execute(Direction dir) {
		if (!checkIfPossible(dir)) { return; }
		this->setMove(dir, _gridArray, _prevArray, _tiles);
		this->spawnTile();
	}

	bool Grid::isArrayEmpty() {
		for (int y = 0; y < _numberOfTiles; y++) {
			for (int x = 0; x < _numberOfTiles; x++) {
				if (_gridArray[y][x] != 0) {
					return false;
				}
			}
		}
		return true;
	}

	std::vector<std::vector<int>> Grid::getArray() {
		return _gridArray;
	}

	std::vector<std::vector<int>> Grid::getPrev() {
		return _prevArray;
	}

	void Grid::addToScore(int value) {
		score += value;
		std::cout << "Adding to score.\n";
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////													Tile													////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void Tile::init(GameDataPtr data, int value, sf::Vector2i pos, sf::Vector2f position, float tileSize) {
		this->_data = data;
		this->_value = value;
		this->_pos = pos;
		this->newPos = pos;
		this->_tileSize = tileSize;
		std::string fileName = std::to_string(value);

		// Set texture
		this->_body.setTexture(this->_data->assets.getTexture(fileName));

		// Set origin
		this->_body.setOrigin(sf::Vector2f(this->_body.getGlobalBounds().width / 2.0f, this->_body.getGlobalBounds().height / 2.0f));

		// Scale sprite
		this->_targetScale = sf::Vector2f((this->_tileSize - (0.06f * this->_tileSize)) / (_body.getGlobalBounds().height), (this->_tileSize - (0.06f * this->_tileSize)) / (_body.getGlobalBounds().height));

		this->_body.setScale(this->_targetScale);

		// Set position
		this->_body.setPosition(position);
	}

	sf::Vector2f Tile::_calculateVector(sf::Vector2i vector) {
		return sf::Vector2f((vector.x - this->_pos.x) * _tileSize, (vector.y - this->_pos.y) * _tileSize);
	}

	void Tile::move(int div) {
		sf::Vector2f vector = _calculateVector(newPos);
		_body.move(vector.x / div, vector.y / div);
	}

	void Tile::setNewPos() {
		_pos = newPos;
	}

	void Tile::spawnTile(int div) {
		float scale = _targetScale.x / div;
		_body.setScale(sf::Vector2f(_body.getScale().x + scale, _body.getScale().y + scale));
	}
	
	void Tile::setScale(float scale) {
		_body.setScale(sf::Vector2f(scale, scale));
	}

	void Tile::draw(sf::RenderWindow& window) {
		window.draw(this->_body);
	}
}