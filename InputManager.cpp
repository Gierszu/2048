#include "InputManager.h"

namespace game {
	bool InputManager::isSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window) {
		if (sf::Mouse::isButtonPressed(button)) {
			sf::IntRect tempRect((object.getPosition().x - (object.getGlobalBounds().width / 2)), (object.getPosition().y - (object.getGlobalBounds().height / 2)), object.getGlobalBounds().width, object.getGlobalBounds().height);
			if (tempRect.contains(sf::Mouse::getPosition(window))) {
				return true;
			}
		}
		return false;
	}

	bool InputManager::isMouseOver(sf::Sprite object, sf::RenderWindow& window) {
		sf::IntRect tempRect((object.getPosition().x - (object.getGlobalBounds().width / 2)), (object.getPosition().y - (object.getGlobalBounds().height / 2)), object.getGlobalBounds().width, object.getGlobalBounds().height);
		if (tempRect.contains(sf::Mouse::getPosition(window))) {
				return true;
		}
		return false;
	}

	sf::Vector2i InputManager::getMousePosition(sf::RenderWindow& window) {
		return sf::Mouse::getPosition(window);
	}
}