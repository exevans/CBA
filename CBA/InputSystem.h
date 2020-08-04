#ifndef INPUTSYS_H
#define INPUTSYS_H

#include "System.h"
#include "KeyBinding.h"
#include "Window.h"

class InputSystem : public System
{
public:
	InputSystem(SystemManager*);
	void update(float) override;
	void onNotify(const Message&) override;
	void handleEvent(const EntityID&, const EntityEvent&) override;

private:
	//Window* _window;

	//TEMP GUI
	/*sf::RectangleShape _equipedItemTextBox;
	sf::Text _equipedItemText;
	sf::Font font;*/
};

#endif