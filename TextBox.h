#pragma once

#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>
#include <string>



namespace game {
	class TextBox {
	public:
		TextBox() {}
		~TextBox() {}

		void init(GameDataPtr data, sf::Vector2f size, sf::Vector2f pos);
		sf::FloatRect getGlobalBounds();
		sf::Vector2f getPosition();

		void changeText(std::string text);
		void changeTitle(std::string title);
		void scaleToBox(sf::Text& text, sf::RectangleShape& box);
		void draw(sf::RenderWindow& window);

		std::string getText();

	private:
		GameDataPtr _data;

		sf::Sprite _body;

		sf::Text _title;
		sf::Text _text;
		sf::RectangleShape _textRect;
		sf::RectangleShape _titleRect;
	};
}