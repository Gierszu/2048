#pragma once

namespace game {
	class State {
	public:
		virtual void init() = 0;
		virtual void handleInput() = 0;

		virtual void update(float frameDif) = 0;
		virtual void draw(float frameDif) = 0;

		virtual void pause() {}
		virtual void resume() {}
	};
}