#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <thread>

#include "DEFINITIONS.h"
#include "Game.h"

namespace game {
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////													Tile													////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	class Tile {
	public:
		Tile() {};
		~Tile() {};

		void init(GameDataPtr data,int value, sf::Vector2i pos, sf::Vector2f position, float tileSize);
		
		void move(int div = 10);
		void spawnTile(int div = 10);
		void setScale(float scale);
		void setNewPos();
		
		int getID() { return this->_ID; }
		int getValue() { return this->_value; }
		sf::Vector2f getTargetScale() { return this->_targetScale; }

		sf::Vector2i newPos;

		void draw(sf::RenderWindow& window);
	private:
		GameDataPtr _data;

		sf::Vector2f _calculateVector(sf::Vector2i vector);

		int _value;
		int _ID;

		float _tileSize;
		
		sf::Vector2f _targetScale;
		sf::Vector2i _pos;

		sf::Sprite _body;
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////													Grid													////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Grid {
	public:
		Grid() {}
		~Grid() {};

		void init(GameDataPtr data, sf::Vector2f pos, float size, int number);
		void init(GameDataPtr data, sf::Vector2f pos, float size, int number, std::vector<std::vector<int>> array, std::vector<std::vector<int>> prev);
		void initTiles();

		void setMove(Direction dir, std::vector<std::vector<int>>& array, std::vector<std::vector<int>>& prev, std::vector<Tile>& tiles, bool isAdding = false);
		bool checkIfPossible(Direction dir);
		void setTiles();
		void spawnTile();
		void animate(int div = 10);
		void execute(Direction dir);
		void setPrevToArr();
		void setArrToPrev();
		void moveToTiles();
		bool isArrayEmpty();
		bool isThere2048();
		void addToScore(int value);
		std::vector<std::vector<int>> getArray();
		std::vector<std::vector<int>> getPrev();

		int score = 0;

		sf::Vector2f getPosition() { return this->_body.getPosition(); };

		void draw(sf::RenderWindow& window);
	private:
		GameDataPtr _data;

		std::vector<Tile> _tiles;
		std::vector<Tile>_spawnedTiles;
		std::vector<Tile> _poppedTiles;

		int _numberOfTiles;

		std::vector<std::vector<int>> _gridArray;
		std::vector<std::vector<int>> _prevArray;

		float _tileSize;
		float _gridSize;

		sf::Sprite _body;
		sf::Sprite _zero;
		std::vector<std::vector<sf::Sprite>> _gridZero;
	};
}