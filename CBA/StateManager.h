#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <vector>
#include <unordered_map>
#include <functional>

#include "StateType.h"
//#include "State.h"
//#include "State_Intro.h"
#include "SharedContext.h"

class State;

using TypeContainer = std::vector<StateType>;
using StateContainer = std::vector<std::pair<StateType, State*>>;
using StateFactory = std::unordered_map<StateType, std::function<State*(void)>>;

class StateManager {
public:
	StateManager(SharedContext* _shared);
	~StateManager();

	void update(const float);
	void draw();

	SharedContext* getShared();
	bool hasState(const StateType&);

	void requestSwitchTo(const StateType&);
	void remove(const StateType&);
	void processRequests();

private:
	void switchTo(const StateType&);
	void createState(const StateType&);
	void removeState(const StateType&);
	void sortTrancendence();

	template<class T>
	void registerState(const StateType& type){
		_stateFactory[type] = [this]() -> State*
		{
			return new T(this);
		};
	}

	SharedContext* _sharedContext;
	TypeContainer _toRemove;
	StateContainer _states;
	StateFactory _stateFactory;

	bool _switchStateRequest;
	StateType _newState;
};

#endif