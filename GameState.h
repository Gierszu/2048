#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "State.h"
#include "Game.h"
#include "DEFINITIONS.h"

#include "Grid.h"
#include "TextBox.h"

namespace game {
	class GameState : public State {
	public:
		GameState(GameDataPtr data);

		void init();
		void handleInput();
		void update(float frameDif);
		void draw(float frameDif);
		void resume();

		void updateScore();
		void updateHighscore();
		void updateFiles();

	private:

		GameDataPtr _data;

		sf::Sprite _pauseButton;
		sf::Sprite _prevButton;
		
		TextBox _scoreBoard;
		TextBox _highscoreBoard;
		sf::Sprite _title;

		sf::RectangleShape _fade;

		int _gameState;
		int _numberOfTiles;

		int _opacity;

		int _score;
		int _highscore;

		float _pauseScale;
		float _prevScale;

		sf::Clock _clock;
		sf::Clock _fadeClock;
		sf::Clock _moveClock;

		Grid _grid;
	};
}
