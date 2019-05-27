#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "State.h"
#include "Game.h"
#include "DEFINITIONS.h"

namespace game {
	class PauseState : public State {
	public:
		PauseState(GameDataPtr data);

		void init();
		void handleInput();
		void update(float frameDif);
		void draw(float frameDif);

		void resetFiles();

	private:
		GameDataPtr _data;

		sf::Sprite _resumeButton;
		sf::Sprite _homeButton;
		sf::Sprite _restartButton;

		sf::RectangleShape _fade;
		int _opacity = 255;

		float _resumeScale, _restartScale, _homeScale;
		
		sf::Clock _clock;
		sf::Clock _fadeClock;
		sf::Clock _exitClock;
	};
}