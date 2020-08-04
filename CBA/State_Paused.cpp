#include <SFML/Graphics.hpp>

#include "State_Paused.h"
#include "StateManager.h"
#include "SharedContext.h"
#include "EventManager.h"
#include "Window.h"

void State_Paused::onCreate() {
	setTransparent(true);
	setTrancendent(true);

	sf::Vector2u winSize = _stateManager->getShared()->_window->getWinSize();

	_background.setSize(sf::Vector2f(winSize));
	_background.setFillColor(sf::Color(0, 0, 0, 150));

	if (!_font.loadFromFile("Assets/Fonts/BernHC.ttf")) {
		std::cout << "Could not load font from file!" << std::endl;
	}

	_text.setFont(_font);
	_text.setCharacterSize(30);
	_text.setFillColor(sf::Color::White);
	_text.setString("Paused");
	_text.setPosition(winSize.x/2 - _text.getGlobalBounds().width/2, winSize.y/2 - _text.getGlobalBounds().height/2);

	EventManager* evManag = _stateManager->getShared()->_eventManager;
	evManag->addCallBack(StateType::Paused, "Key_P", &State_Paused::returnToGame, this);
}

void State_Paused::onDestroy() {
	EventManager* evManag = _stateManager->getShared()->_eventManager;
	evManag->removeCallback(StateType::Paused, "Key_P");
}

void State_Paused::update(float deltaTime) {

}

void State_Paused::draw() {
	_stateManager->getShared()->_window->draw(_background);
	_stateManager->getShared()->_window->draw(_text);
}

void State_Paused::activate() {}

void State_Paused::deactivate() {}

void State_Paused::returnToGame(EventDetails* test) {
	_stateManager->requestSwitchTo(StateType::Game);
}