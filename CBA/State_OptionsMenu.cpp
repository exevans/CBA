#include "State_OptionsMenu.h"
#include "StateManager.h"
#include "Window.h"
#include "EventManager.h"
#include "EventManager.h"

void State_OptionsMenu::onCreate() {
	sf::Vector2u winSize = _stateManager->getShared()->_window->getWinSize();

	EventManager* evManag = _stateManager->getShared()->_eventManager;
	evManag->addCallBack(StateType::OptionsMenu, "Key_Escape", &State_OptionsMenu::continueToMain, this);
}

void State_OptionsMenu::onDestroy() {
	EventManager* evManag = _stateManager->getShared()->_eventManager;
	evManag->removeCallback(StateType::OptionsMenu, "Key_Escape");
}

void State_OptionsMenu::update(float deltaTime) {
	
}

void State_OptionsMenu::draw() {

}

void State_OptionsMenu::activate() {}

void State_OptionsMenu::deactivate() {}

void State_OptionsMenu::continueToMain(EventDetails* details) {
	_stateManager->requestSwitchTo(StateType::MainMenu);
}