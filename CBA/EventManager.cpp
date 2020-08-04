#include "EventManager.h"    
#include <fstream>
#include <sstream>

EventDetails::EventDetails(const std::string& name) : _name(name) {
	clear();
}

void EventDetails::clear() {
	_size = sf::Vector2i(0, 0);
	_textEntered = 0;
	_mouse = sf::Vector2i(0, 0);
	_mouseWheelDelta = 0;
	_keyCode = -1;
}

Binding::Binding(const std::string& name) : _name(name), _details(name), c(0) {

}

void Binding::bindEvent(EventType type, EventInfo info = EventInfo()) {
	_events.emplace_back(type, info);
}


EventManager::EventManager() : _hasFocus(true) {
	loadBindings();
}

EventManager::~EventManager() {
	for (auto &it : _bindings) {
		delete it.second;
		it.second = nullptr;
	}
}

bool EventManager::addBinding(Binding* binding) {
	if (_bindings.find(binding->_name) != _bindings.end())
		return false;

	return _bindings.emplace(binding->_name, binding).second;
}

bool EventManager::removeBinding(std::string name) {
	auto it = _bindings.find(name);
	if (it == _bindings.end())
		return false;

	delete it->second;
	_bindings.erase(it);
	return true;
}


void EventManager::setFocus(const bool& focus) {
	_hasFocus = focus;
}

void EventManager::setCurrentState(const StateType& current) {
	_currentState = current;
}

void EventManager::loadBindings() {
	std::string delim = ":";
	std::ifstream file;

	file.open("Assets/bindings.cfg.txt");
	if (!file.is_open()) {
		std::cout << "Could not load bindings.cfg" << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::stringstream keyStream(line);
		std::string callbackName;
		keyStream >> callbackName;
		Binding* bind = new Binding(callbackName);

		while (!keyStream.eof()) {
			//produce string version of the line
			std::string keyVal;
			keyStream >> keyVal;
			int start = 0;
			int end = keyVal.find(delim);
			//end = val where colon is
			//if the delim was not found
			if (end == std::string::npos) {
				delete bind;
				bind = nullptr;
				break;
			}
			EventType type = EventType(std::stoi(keyVal.substr(start, end - start)));
			int code = std::stoi(keyVal.substr(end + delim.length(), keyVal.find(delim, end + delim.length())));

			EventInfo eventInfo(code);

			bind->bindEvent(type, eventInfo);
		}

		if (!addBinding(bind)) {
			delete bind;
		}
		bind = nullptr;
	}
	//close the bindings file
	file.close();
}

bool EventManager::removeCallback(StateType state, const std::string& name) {
	auto itr = _callbacks.find(state);
	if (itr == _callbacks.end()) {
		return false;
	}
	auto itr2 = itr->second.find(name);
	if (itr2 == itr->second.end()) {
		return false;
	}
	itr->second.erase(itr2);
	return true;
}

void EventManager::handleEvent(sf::Event event) {
	for (auto &b_it : _bindings) {
		Binding* bind = b_it.second;
		for (auto e_it : bind->_events) {
			EventType sfmlEvent = (EventType)event.type;
			if (e_it.first != sfmlEvent)
				continue;
			if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp) {
				if (e_it.second._code == event.key.code) {
					if (bind->_details._keyCode != -1) {
						bind->_details._keyCode = e_it.second._code;
					}
					++(bind->c);
					break;
				}
			}
			else if (sfmlEvent == EventType::MButtonDown || sfmlEvent == EventType::MButtonUp) {
				if (e_it.second._code == event.mouseButton.button) {
					bind->_details._mouse.x = event.mouseButton.x;
					bind->_details._mouse.y = event.mouseButton.y;
					if (bind->_details._keyCode != -1) {
						bind->_details._keyCode = e_it.second._code;
					}
					++(bind->c);
					break;
				}
			}
			/*else if (sfmlEvent == EventType::MouseWheel || sfmlEvent == EventType::WindowResized || sfmlEvent == EventType::TextEntered) {
				if (sfmlEvent == EventType::MouseWheel)
					bind->_details._mouseWheelDelta = event.mouseWheel.delta;
				else if (sfmlEvent == EventType::WindowResized) {
					bind->_details._size.x = event.size.width;
					bind->_details._size.y = event.size.height;
				}
				else if (sfmlEvent == EventType::TextEntered) {
					bind->_details._textEntered = event.text.unicode;
				}
				++(bind->c);
			}*/
			else if (sfmlEvent == EventType::JoystickButtonDown || sfmlEvent == EventType::JoystickButtonUp) {
				if (e_it.second._code == event.joystickButton.button) {
					if (bind->_details._keyCode != -1) {
						bind->_details._keyCode = e_it.second._code;
					}
					++(bind->c);
					break;
				}
			}
			else {
				++(bind->c);
			}
		}
	}
}

void EventManager::update() {
	if (!this->_hasFocus) {
		return;
	}

	for (auto& b_it : this->_bindings) {
		Binding* bind = b_it.second;
		for (auto& e_it : bind->_events) {
			switch (e_it.first) {
			case (EventType::Keyboard) : 
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e_it.second._code))) {
					if (bind->_details._keyCode != -1) {
						bind->_details._keyCode = e_it.second._code;
					}
					++(bind->c);
				}
				break;
			case (EventType::Mouse) :
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button(e_it.second._code))) {
					if (bind->_details._keyCode != -1) {
						bind->_details._keyCode = e_it.second._code;
					}
					++(bind->c);
				}
				break;
			case (EventType::Joystick) :
				/*if (sf::Joystick::isButtonPressed(0, e_it.second._code)) {
					bool test = true;
				}
				if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y)) {
					bool test = true;
				}*/
				//TODO
				break;
			}
		}

		//check callbacks from input triggers
		if (bind->_events.size() == bind->c){
			auto stateCallbacks = _callbacks.find(_currentState);
			if (stateCallbacks != _callbacks.end()){
				auto callItr = stateCallbacks->second.find(bind->_name);
				if (callItr != stateCallbacks->second.end()){
					// Pass in information about events to the callback.
					callItr->second(&bind->_details);
				}
			}

			auto otherCallbacks = _callbacks.find(StateType(0));
			if (otherCallbacks != _callbacks.end()){
				auto callItr = otherCallbacks->second.find(bind->_name);
				if (callItr != otherCallbacks->second.end()){
					// Pass in information about events.
					callItr->second(&bind->_details);
				}
			}
		}
		bind->c = 0;
		bind->_details.clear();
	}
}

sf::Vector2i EventManager::getMousePos(sf::RenderWindow* win = nullptr) {
	return (win ? sf::Mouse::getPosition(*win) : sf::Mouse::getPosition());
}