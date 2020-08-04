#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "EventType.h"
#include "StateType.h"

#include <functional>
#include <map>
#include <unordered_map>

struct EventInfo {
	EventInfo() { _code = 0; }
	EventInfo(int l_event) { _code = l_event; }
	union {
		int _code;
	};
};

struct EventDetails {
	EventDetails(const std::string&);
	std::string _name;

	void clear();

	sf::Vector2i _size;
	sf::Uint32 _textEntered;
	sf::Vector2i _mouse;
	int _mouseWheelDelta;
	int _keyCode;
};

struct Binding {
	Binding(const std::string& name);
	void bindEvent(EventType, EventInfo);

	std::vector<std::pair<EventType, EventInfo>> _events;
	std::string _name;
	int c;

	EventDetails _details;
};

using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;
using Callbacks = std::unordered_map<StateType, CallbackContainer>;

class EventManager {
public:
	EventManager();
	~EventManager();

	bool addBinding(Binding *binding);
	bool removeBinding(std::string name);
	void setFocus(const bool& focus);
	void setCurrentState(const StateType&);

	template<class T>
	bool addCallBack(StateType state, const std::string& name, void(T::*func)(EventDetails*), T* instance) {
		auto itr = _callbacks.emplace(state, CallbackContainer()).first;
		auto temp = std::bind(func, instance, std::placeholders::_1);
		return itr->second.emplace(name, temp).second;
	}

	bool removeCallback(StateType, const std::string&);

	void handleEvent(sf::Event);
	void update();

	sf::Vector2i getMousePos(sf::RenderWindow*);

private:
	void loadBindings();

	std::unordered_map<std::string, Binding*> _bindings;
	Callbacks _callbacks;
	bool _hasFocus;
	StateType _currentState;
};

#endif