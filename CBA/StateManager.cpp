#include "StateManager.h"
#include "State.h"
#include "State_Intro.h"
#include "State_MainMenu.h"
#include "State_Game.h"
#include "State_OptionsMenu.h"
#include "State_Dialogue.h"
#include "State_Paused.h"
#include "EventManager.h"

StateManager::StateManager(SharedContext* shared) : _sharedContext(shared), _switchStateRequest(false) {
	registerState<State_Intro>(StateType::Intro);
	registerState<State_MainMenu>(StateType::MainMenu);
	registerState<State_Game>(StateType::Game);
	registerState<State_Paused>(StateType::Paused);
	registerState<State_OptionsMenu>(StateType::OptionsMenu);
	registerState<State_Dialogue>(StateType::Dialogue);
}

StateManager::~StateManager() {
	std::cout << "started to destryot state manager" << std::endl;

	for (auto& it : this->_states) {
		it.second->onDestroy();
		delete it.second;
		it.second = nullptr;
	}

	//_sharedContext->_luaManager->purge();
	//_sharedContext->_entityManager->purge();
}

void StateManager::draw() {
	if (_states.empty()) {
		return;
	}

	if (_states.back().second->isTransparent() && _states.size() > 1) {
		auto itr = _states.end();
		while (itr != _states.begin()) {
			//itr first point to spot after last
			if (itr != _states.end()) {
				if (!itr->second->isTransparent()) {
					break;
				}
			}
			--itr;
		}
		for (; itr != _states.end(); ++itr) {
			_sharedContext->_window->getRenderWindow()->setView(itr->second->getView());
			itr->second->draw();
		}
	}
	else {
		_sharedContext->_window->getRenderWindow()->setView(_states.back().second->getView());
		_states.back().second->draw();
	}
}

void StateManager::update(const float delta) {
	if (_states.empty()) {
		return;
	}

	//update all states till not trans
	for (auto itr = _states.end() - 1; itr >= _states.begin(); --itr) {
		itr->second->update(delta);

		//check if keep going
		if (!itr->second->isTrancendent()) 
			return;
	}
}

SharedContext* StateManager::getShared() {
	return _sharedContext;
}

bool StateManager::hasState(const StateType& type) {
	//check if currently contained
	for (auto itr = _states.begin(); itr != _states.end(); ++itr) {
		if (itr->first == type) {
			//contined check not to be removed
			auto removed = std::find(_toRemove.begin(), _toRemove.end(), type);
			if (removed != _toRemove.end()) {
				return false;
			}
			else {
				return true;
			}
		}
	}
	return false;
}

void StateManager::requestSwitchTo(const StateType& type) {
	_switchStateRequest = true;
	_newState = type;
}

void StateManager::remove(const StateType& type) {
	_toRemove.push_back(type);
}

void StateManager::processRequests() {
	//switch state if needed
	if (_switchStateRequest) {
		switchTo(_newState);
		_switchStateRequest = false;
	}

	//check for removals
	while (_toRemove.begin() != _toRemove.end()) {
		removeState(*_toRemove.begin());
		_toRemove.erase(_toRemove.begin());
	}
}


void StateManager::switchTo(const StateType& type) {
	//set current event manager state
	EventManager* eventMang = _sharedContext->_eventManager;
	eventMang->setCurrentState(type);

	//if state already exists (differnt layer) activate it
	for (auto itr = _states.begin(); itr != _states.end(); ++itr) {
		if (itr->first == type) {
			_states.back().second->deactivate();
			StateType tmp_type = itr->first;
			State* tmp_state = itr->second;
			_states.erase(itr);
			_states.emplace_back(tmp_type, tmp_state);
			tmp_state->activate();
			_sharedContext->_window->getRenderWindow()->setView(tmp_state->getView());
			return;
		}
	}

	//otherwise just add to back
	if (!_states.empty()) {
		_states.back().second->deactivate();
	}
	createState(type);
	_states.back().second->activate();

	_sharedContext->_window->getRenderWindow()->setView(_states.back().second->getView());
}

void StateManager::createState(const StateType& type) {
	const auto& newStateIt = _stateFactory.find(type);
	if (newStateIt == _stateFactory.end()) {
		//not registered
		return;
	}
	//create the state
	State* state = newStateIt->second();
	//init view of state
	state->getView() = _sharedContext->_window->getRenderWindow()->getDefaultView();
	_states.emplace_back(type, state);
	state->onCreate();
}

void StateManager::removeState(const StateType& type) {
	for (auto itr = _states.begin(); itr != _states.end(); ++itr) {
		//if type being looked for
		if (itr->first == type) {
			itr->second->onDestroy();	
			delete itr->second;
			_states.erase(itr);
			break;
		}
	}
}