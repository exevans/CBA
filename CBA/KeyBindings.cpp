#include "KeyBinding.h"

std::vector<std::string> KeyBinder::_pendingEvent;
//std::map<std::string, sf::Keyboard::Key> _keyboardBindings;

KeyBinder::KeyBinder()
{
	InputEvent _input;

	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::Escape;
	_keyboardInputs["Exit"] = _input;

	_input._inputType = Input::Joystick;
	_input._eventType = sf::Event::JoystickButtonPressed;
	_input._joystickButton = 7;
	_joystickInputs["Exit"] = _input;

	_input._inputType = Input::Mouse;
	_input._eventType = sf::Event::MouseButtonPressed;
	_input._mouseButton = sf::Mouse::Left;
	_mouseInputs["Activate"] = _input;

	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::Q;
	_keyboardInputs["Activate"] = _input;

	_input._inputType = Input::Joystick;
	_input._eventType = sf::Event::JoystickButtonPressed;
	_input._joystickButton = 2;
	_joystickInputs["Activate"] = _input;

	_input._inputType = Input::Joystick;
	_input._eventType = sf::Event::JoystickButtonPressed;
	_input._joystickButton = 0;
	_joystickInputs["Interact"] = _input;

	_input._inputType = Input::Joystick;
	_input._eventType = sf::Event::JoystickButtonPressed;
	_input._joystickButton = 5;
	_joystickInputs["NextItem"] = _input;

	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::Down;
	_keyboardInputs["NextItem"] = _input;

	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::Up;
	_keyboardInputs["PreviousItem"] = _input;

	_input._inputType = Input::Joystick;
	_input._eventType = sf::Event::JoystickButtonPressed;
	_input._joystickButton = 4;
	_joystickInputs["PreviousItem"] = _input;

	_input._inputType = Input::Mouse;
	_input._eventType = sf::Event::MouseWheelMoved;
	_input._mouseWheelDelta = 1;
	_mouseInputs["NextItem"] = _input;

	_input._inputType = Input::Mouse;
	_input._eventType = sf::Event::MouseWheelMoved;
	_input._mouseWheelDelta = -1;
	_mouseInputs["PreviousItem"] = _input;
	
	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::E;
	_keyboardInputs["Interact"] = _input;

	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::I;
	_keyboardInputs["OpenInventory"] = _input;

	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::Num1;
	_keyboardInputs["Option1"] = _input;

	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::Num2;
	_keyboardInputs["Option2"] = _input;

	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::Num3;
	_keyboardInputs["Option3"] = _input;

	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::Num4;
	_keyboardInputs["Option4"] = _input;

	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::Num5;
	_keyboardInputs["Option5"] = _input;

	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::I;
	_keyboardInputs["Inventory"] = _input;

	_input._inputType = Input::Keyboard;
	_input._eventType = sf::Event::KeyPressed;
	_input._keyboardKey = sf::Keyboard::F5;
	_keyboardInputs["Save"] = _input;

	/*_input._inputType = Input::Joystick;
	_input._eventType = sf::Event::JoystickButtonPressed;
	_input._joystickButton = 0;
	_joystickInputs["Option1"] = _input;

	_input._inputType = Input::Joystick;
	_input._eventType = sf::Event::JoystickButtonPressed;
	_input._joystickButton = 1;
	_joystickInputs["Option2"] = _input;

	_input._inputType = Input::Joystick;
	_input._eventType = sf::Event::JoystickButtonPressed;
	_input._joystickButton = 2;
	_joystickInputs["Option3"] = _input;

	_input._inputType = Input::Joystick;
	_input._eventType = sf::Event::JoystickButtonPressed;
	_input._joystickButton = 3;
	_joystickInputs["Option4"] = _input;*/

	//this->_keyboardBindings["Up"] = sf::Keyboard::W;
}

bool KeyBinder::TestEvent(std::string k, sf::Event e)
{
	//mouse events
	if (_mouseInputs[k]._inputType == Input::Mouse && _mouseInputs[k]._eventType == e.type && (_mouseInputs[k]._mouseButton == e.mouseButton.button || _mouseInputs[k]._mouseWheelDelta == e.mouseWheel.delta))
	{
		this->addEventToQueue(k);
		return true;
	}
	//keyboard events
	if (_keyboardInputs[k]._inputType == Input::Keyboard && _keyboardInputs[k]._eventType == e.type && _keyboardInputs[k]._keyboardKey == e.key.code)
	{
		this->addEventToQueue(k);
		return true;
	}
	//joystick events
	if (_joystickInputs[k]._inputType == Input::Joystick && _joystickInputs[k]._eventType == e.type && _joystickInputs[k]._joystickButton == e.joystickButton.button)
	{
		this->addEventToQueue(k);
		return true;
	}

	return false;
}


bool KeyBinder::isEventPending(std::string eventName)
{
	for (std::vector<std::string>::iterator i = _pendingEvent.begin(); i != _pendingEvent.end(); ++i)
	{
		if (i->data() == eventName)
		{
			_pendingEvent.erase(i);
			return true;
		}
	}

	return false;
}

void KeyBinder::addEventToQueue(std::string eventName)
{
	_pendingEvent.push_back(eventName);
}

void KeyBinder::clearEvents()
{
	_pendingEvent.clear();
}

/*sf::Keyboard::Key KeyBinder::getKey(std::string keyName) 
{
	return _keyboardBindings[keyName];
}*/