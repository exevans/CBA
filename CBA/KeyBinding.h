#ifndef KEYBIND_H
#define KEYBIND_H

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

namespace Input
{
	enum InputType { Keyboard, Mouse, Joystick };
}

struct InputEvent
{
	Input::InputType _inputType;
	sf::Event::EventType _eventType;
	sf::Keyboard::Key _keyboardKey;
	sf::Mouse::Button _mouseButton;
	float _mouseWheelDelta;
	unsigned int _joystickButton;
};

class KeyBinder
{
public:
	KeyBinder();
	bool TestEvent(std::string, sf::Event);

	static bool isEventPending(std::string);
	void clearEvents();

	//static sf::Keyboard::Key getKey(std::string);

private:
	// for events
	std::map<std::string, InputEvent> _mouseInputs;
	std::map<std::string, InputEvent> _keyboardInputs;
	std::map<std::string, InputEvent> _joystickInputs;

	//for general bindings
	//static std::map<std::string, sf::Keyboard::Key> _keyboardBindings;

	static std::vector<std::string> _pendingEvent;
	void addEventToQueue(std::string);
};

#endif