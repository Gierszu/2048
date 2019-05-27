#include "TextBox.h"

#include <math.h>


namespace game {
	void TextBox::init(GameDataPtr data, sf::Vector2f size, sf::Vector2f pos) {
		this->_data = data;

		// Set textures	& fonts
		this->_body.setTexture(this->_data->assets.getTexture("Text Box"));
		this->_text.setFont(this->_data->assets.getFont("Calm Font"));
		this->_title.setFont(this->_data->assets.getFont("Calm Font"));

		// Set text
		this->_text.setFillColor(sf::Color::Black);
		this->_title.setFillColor(sf::Color::Black);
		this->_titleRect.setFillColor(sf::Color::Transparent);
		this->_textRect.setFillColor(sf::Color::Transparent);

		// Set origins
		this->_body.setOrigin(sf::Vector2f(this->_body.getGlobalBounds().width / 2.0f, this->_body.getGlobalBounds().height / 2.0f));
		this->_text.setOrigin(_text.getLocalBounds().width / 2.0f, _text.getLocalBounds().height / 2.0f);
		this->_title.setOrigin(_title.getLocalBounds().width / 2.0f, _title.getLocalBounds().height / 2.0f);
		this->_textRect.setOrigin(_textRect.getLocalBounds().width / 2.0f, _textRect.getLocalBounds().height / 2.0f);
		this->_titleRect.setOrigin(_titleRect.getLocalBounds().width / 2.0f, _titleRect.getLocalBounds().height / 2.0f);

		// Scale sprites & character size
		this->_body.setScale(sf::Vector2f(size.x / (_body.getGlobalBounds().height), size.y / (_body.getGlobalBounds().height)));
		this->_titleRect.setSize(sf::Vector2f(size.x, (size.y * 0.36f)));
		this->_textRect.setSize(sf::Vector2f(size.x, (size.y * 0.64f)));

		// Set positions
		this->_body.setPosition(pos);
		this->_titleRect.setPosition(sf::Vector2f((pos.x), (pos.y - 0.32f * size.y)));
		this->_textRect.setPosition(sf::Vector2f((pos.x), (pos.y + 0.18f * size.y)));
		this->_text.setPosition(_textRect.getPosition());
		this->_title.setPosition(_titleRect.getPosition());
	}

	void TextBox::draw(sf::RenderWindow& window) {
		window.draw(this->_body);
		window.draw(this->_text);
		window.draw(this->_title);
	}

	void TextBox::changeText(std::string text) {
		this->_text.setString(text);
		scaleToBox(_text, _textRect);
		this->_text.setOrigin(_text.getLocalBounds().width / 2.0f, _text.getLocalBounds().height / 2.0f);
	}

	void TextBox::changeTitle(std::string title) {
		this->_title.setString(title);
		scaleToBox(_title, _titleRect);
		this->_title.setOrigin(_title.getLocalBounds().width / 2.0f, _title.getLocalBounds().height / 2.0f);
	}

	void TextBox::scaleToBox(sf::Text& text, sf::RectangleShape& box) {
		float sizeX = 0.7f * box.getGlobalBounds().width;
		float sizeY = 0.7f * box.getGlobalBounds().height;
		float maxSize = 100;
		
		text.setCharacterSize(std::min(sizeY, maxSize));

		while (text.getGlobalBounds().width > sizeX) {
			text.scale(sf::Vector2f(0.99f, 0.99f));
		}
	}

	sf::FloatRect TextBox::getGlobalBounds() {
		return this->_body.getGlobalBounds();
	}

	sf::Vector2f TextBox::getPosition() {
		return this->_body.getPosition();
	}

	std::string TextBox::getText() {
		return this->_text.getString();
	}
}