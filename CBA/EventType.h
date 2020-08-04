#ifndef EVENT_TYPE_H
#define EVENT_TYPE_H

#include<SFML/Graphics.hpp>

enum class EventType {
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	JoystickButtonDown = sf::Event::JoystickButtonPressed,
	JoystickButtonUp = sf::Event::JoystickButtonReleased,
	Keyboard = sf::Event::Count + 1, 
	Mouse, 
	Joystick
};

#endif