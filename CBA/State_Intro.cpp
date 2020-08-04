#include "State_Intro.h"
#include "StateManager.h"
#include "SharedContext.h"
#include "EventManager.h"
#include "Window.h"

void State_Intro::onCreate() {
	sf::Vector2u winSize = _stateManager->getShared()->_window->getWinSize();

	//throw error of not loaded
	if (!_introTexture.loadFromFile("Assets/Textures/Menu/Intro/logo.png")) {
		std::cout << "Could not load intro pic from file!" << std::endl;
	}
	_introSprite.setTexture(_introTexture);
	_introSprite.setOrigin(_introTexture.getSize().x / 2.0f, _introTexture.getSize().y / 2.0f);
	_introSprite.setPosition(winSize.x / 2.0, winSize.y/2);
	_introSprite.setScale(0.30, 0.30);

	_shape.setFillColor(sf::Color(255, 0, 0, 180));
	_shape.setSize(sf::Vector2f(500.0f, 50.0f));
	_shape.setPosition(sf::Vector2f(100.0f, 100.0f));

	EventManager* evManag = _stateManager->getShared()->_eventManager;
	evManag->addCallBack(StateType::Intro, "Intro_Continue", &State_Intro::continueToMain, this);
}

void State_Intro::onDestroy() {
	EventManager* evManag = _stateManager->getShared()->_eventManager;
	evManag->removeCallback(StateType::Intro, "Intro_Continue");
}

void State_Intro::update(float deltaTime) {
	if (_introSprite.getScale().x < 0.45) {
		_introSprite.setScale(sf::Vector2f(_introSprite.getScale().x + (0.02 * deltaTime), _introSprite.getScale().y + (0.02 * deltaTime)));
	}
}

void State_Intro::draw() {
	_stateManager->getShared()->_window->draw(_introSprite);
	_stateManager->getShared()->_window->draw(_shape);
}

void State_Intro::activate() {}

void State_Intro::deactivate() {}

void State_Intro::continueToMain(EventDetails* test) {
	_stateManager->requestSwitchTo(StateType::MainMenu);
	_stateManager->remove(StateType::Intro);
}