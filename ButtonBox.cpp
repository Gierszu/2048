#include "ButtonBox.h"
#include "DEFINITIONS.h"

namespace game {

	void ButtonBox::init(GameDataPtr data, std::string name) {
		_data = data;
		
		// Set textures
		_body.setTexture(_data->assets.getTexture(name));
		_resumeButton.setTexture(_data->assets.getTexture("Resume Button"));
		_restartButton.setTexture(_data->assets.getTexture("Restart Button"));
		_homeButton.setTexture(_data->assets.getTexture("Home Button"));

		// Set origins
		_body.setOrigin(_body.getLocalBounds().width / 2.0f, _body.getLocalBounds().height / 2.0f);
		_resumeButton.setOrigin(_resumeButton.getLocalBounds().width / 2.0f, _resumeButton.getLocalBounds().height / 2.0f);
		_restartButton.setOrigin(_restartButton.getLocalBounds().width / 2.0f, _restartButton.getLocalBounds().height / 2.0f);
		_homeButton.setOrigin(_homeButton.getLocalBounds().width / 2.0f, _homeButton.getLocalBounds().height / 2.0f);

		// Scale sprites
		_body.setScale((SCREEN_WIDTH * 0.7f) / _body.getGlobalBounds().width, (SCREEN_HEIGHT * 0.7466667f) / _body.getGlobalBounds().height);
		_resumeButton.setScale((_body.getGlobalBounds().width * 0.15f) / _resumeButton.getLocalBounds().width, (_body.getGlobalBounds().width * 0.15f) / _resumeButton.getLocalBounds().width);
		_restartButton.setScale(_resumeButton.getScale());
		_homeButton.setScale(_resumeButton.getScale());

		_targetScale = _restartButton.getScale().x;

		// Set positions
		_body.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
		_restartButton.setPosition(_body.getPosition().x, (_body.getPosition().y + ((1.0f / 6.0f) * _body.getGlobalBounds().height)));
		_resumeButton.setPosition((_restartButton.getPosition().x - (_restartButton.getGlobalBounds().width * 1.5f)), _restartButton.getPosition().y);
		_homeButton.setPosition((_restartButton.getPosition().x + (_restartButton.getGlobalBounds().width * 1.5f)), _restartButton.getPosition().y);
	}

	sf::FloatRect ButtonBox::getGlobalBounds() {
		return _body.getGlobalBounds();
	}
	sf::Vector2f ButtonBox::getPosition() {
		return _body.getPosition();
	}

	void ButtonBox::draw(sf::RenderWindow& window) {
		window.draw(_body);
		window.draw(_resumeButton);
		window.draw(_restartButton);
		window.draw(_homeButton);
	}
	
	int ButtonBox::handleInput() {
		sf::Event event;
		while (this->_data->window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::EventType::MouseButtonReleased:
				if (sf::Mouse::Left == event.mouseButton.button) {

					// Resume button released
					if (this->_data->input.isMouseOver(this->_resumeButton, this->_data->window)) {
						return STATE_PLAYING;
					}

					// Restart button released
					if (this->_data->input.isMouseOver(this->_restartButton, this->_data->window)) {
						return STATE_RESTART;
					}

					// Home button released
					if (this->_data->input.isMouseOver(this->_homeButton, this->_data->window)) {
						return STATE_HOME;
					}
				}
				break;
			}
		}
		return STATE_PAUSED;
	}
	
	void ButtonBox::update() {
		// Button animations

		if (this->_data->input.isMouseOver(_resumeButton, this->_data->window)) {
			if (this->_data->input.isSpriteClicked(_resumeButton, sf::Mouse::Left, this->_data->window)) {
				this->_resumeButton.setScale(_targetScale, _targetScale);
			}
			else if (this->_resumeButton.getScale().x < 1.2f * _targetScale) {
				this->_resumeButton.scale(1.05f, 1.05f);
			}
		}

		if (!this->_data->input.isMouseOver(_resumeButton, this->_data->window)) {
			if (this->_resumeButton.getScale().x > _targetScale) {
				this->_resumeButton.scale(0.95f, 0.95f);
			}
		}

		if (this->_data->input.isMouseOver(_restartButton, this->_data->window)) {
			if (this->_data->input.isSpriteClicked(_restartButton, sf::Mouse::Left, this->_data->window)) {
				this->_restartButton.setScale(_targetScale, _targetScale);
			}
			else if (this->_restartButton.getScale().x < 1.2f * _targetScale) {
				this->_restartButton.scale(1.05f, 1.05f);
			}
		}

		if (!this->_data->input.isMouseOver(_restartButton, this->_data->window)) {
			if (this->_restartButton.getScale().x > _targetScale) {
				this->_restartButton.scale(0.95f, 0.95f);
			}
		}

		if (this->_data->input.isMouseOver(_homeButton, this->_data->window)) {
			if (this->_data->input.isSpriteClicked(_homeButton, sf::Mouse::Left, this->_data->window)) {
				this->_homeButton.setScale(_targetScale, _targetScale);
			}
			else if (this->_homeButton.getScale().x < 1.2f * _targetScale) {
				this->_homeButton.scale(1.05f, 1.05f);
			}
		}

		if (!this->_data->input.isMouseOver(_homeButton, this->_data->window)) {
			if (this->_homeButton.getScale().x > _targetScale) {
				this->_homeButton.scale(0.95f, 0.95f);
			}
		}
	}
}