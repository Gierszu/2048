#include "StateMachine.h"

namespace game {
	void StateMachine::addState(StatePtr newState, bool isReplacing) {
		this->_isAdding = true;
		this->_isReplacing = isReplacing;
		this->_newState = std::move(newState);
	}

	void StateMachine::removeState() {
		this->_isRemoving = true;
	}

	void StateMachine::processStateChanges() {
		if (this->_isRemoving && !this->_states.empty()) {
			this->_states.pop();
			if (!this->_states.empty()) {
				this->_states.top()->resume();
			}
			this->_isRemoving = false;
		}
		if (this->_isAdding) {
			if (!this->_states.empty()) {
				if (this->_isReplacing) {
					this->_states.pop();
				}
				else {
					this->_states.top()->pause();
				}
			}
			this->_states.push(std::move(this->_newState));
			this->_states.top()->init();
			this->_isAdding = false;
		}
	}

	StatePtr& StateMachine::getActiveState() {
		return this->_states.top();
	}
}
