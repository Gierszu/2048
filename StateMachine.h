#pragma once

#include <memory>
#include <stack>

#include "State.h"

namespace game {
	typedef std::unique_ptr<State> StatePtr;
	
	class StateMachine
	{
	public:
		StateMachine() {}
		~StateMachine() {}

		void addState(StatePtr newState, bool isReplacing = true);
		void removeState();
		void processStateChanges();

		StatePtr& getActiveState();

	private:
		std::stack<StatePtr> _states;
		
		StatePtr _newState;

		bool _isRemoving = false;
		bool _isAdding = false;
		bool _isReplacing = false;
	};
}
