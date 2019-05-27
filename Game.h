#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

#include "StateMachine.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "FileManager.h"

namespace game {
	struct GameData {
		StateMachine machine;
		sf::RenderWindow window;
		AssetManager assets;
		InputManager input;
		FileManager files;
	};

	typedef std::shared_ptr<GameData> GameDataPtr;

	class Game {
	public:
		Game(int width, int height, std::string title);
	private:
		const float frameDif = 1.0f / 60.0f;
		sf::Clock _clock;

		GameDataPtr _data = std::make_shared<GameData>();

		void Run();
	};
}