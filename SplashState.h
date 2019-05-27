#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "State.h"
#include "Game.h"

namespace game {
	class SplashState : public State {
	public:
		SplashState(GameDataPtr data);

		void init();
		void handleInput();
		void update(float frameDif);
		void draw(float frameDif);

	private:
		GameDataPtr _data;

		sf::Clock _clock;
		sf::Clock _fadeClock;

		sf::Sprite _logo;
		sf::RectangleShape _fade;

		int _opacity = 255;
	};
}