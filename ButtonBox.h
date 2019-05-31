#pragma once

#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>
#include <string>

namespace game {
	class ButtonBox {
	public:
		ButtonBox() {}
		~ButtonBox() {}

		void init(GameDataPtr data, std::string name);
		
		sf::FloatRect getGlobalBounds();
		sf::Vector2f getPosition();

		void draw(sf::RenderWindow& window);
		int handleInput();
		void update();
	private:
		GameDataPtr _data;

		sf::Sprite _body;
		sf::Sprite _resumeButton;
		sf::Sprite _restartButton;
		sf::Sprite _homeButton;

		float _targetScale;
	};
}