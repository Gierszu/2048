#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "State.h"
#include "Game.h"

namespace game {
	class MainMenuState : public State {
	public:
		MainMenuState(GameDataPtr data);

		void init();
		void handleInput();
		void update(float frameDif);
		void draw(float frameDif);

	private:
		GameDataPtr _data;

		sf::Clock _clock;
		sf::Clock _fadeClock;
		sf::Clock _gearClock;
		sf::Clock _exitClock;
		
		sf::RectangleShape _fade;
		sf::Sprite _title;
		std::vector<sf::CircleShape> _gears;
		sf::Sprite _playButton;
		sf::Sprite _exitButton;

		int _opacity = 255;
	};
}