#include "State_MainMenu.h"
#include "StateManager.h"
#include "SharedContext.h"
#include "EventManager.h"
#include "Window.h"

void State_MainMenu::onCreate() {
	sf::Vector2u winSize = _stateManager->getShared()->_window->getWinSize();

	if (!_backgroundTexture.loadFromFile("Assets/Textures/Menu/Main/back_00.png")) {
		std::cout << "Could not load intro pic from file!" << std::endl;
	}
	_backgroundSprite.setTexture(_backgroundTexture);
	_backgroundSprite.setPosition(0, 0);

	//throw error of not loaded
	if (!_introTexture.loadFromFile("Assets/Textures/Menu/Intro/introPic.png")) {
		std::cout << "Could not load intro pic from file!" << std::endl;
	}
	_introSprite.setTexture(_introTexture);
	_introSprite.setOrigin(_introTexture.getSize().x / 2.0f, _introTexture.getSize().y / 2.0f);
	_introSprite.setPosition(winSize.x / 2.0, _introSprite.getGlobalBounds().height - 40);


	_shapes.emplace_back(sf::RectangleShape());
	_shapes.emplace_back(sf::RectangleShape());
	_shapes.emplace_back(sf::RectangleShape());

	_texts.emplace_back(sf::Text());
	_texts.emplace_back(sf::Text());
	_texts.emplace_back(sf::Text());

	_shapes[0].setFillColor(sf::Color::Blue);
	_shapes[0].setSize(sf::Vector2f(200.0f, 50.0f));
	_shapes[0].setPosition(sf::Vector2f(winSize.x / 2 - _shapes[0].getGlobalBounds().width / 2, 100.0f));

	_shapes[1].setFillColor(sf::Color::Magenta);
	_shapes[1].setSize(sf::Vector2f(200.0f, 50.0f));
	_shapes[1].setPosition(sf::Vector2f(winSize.x / 2 - _shapes[0].getGlobalBounds().width / 2, 155.0f));

	_shapes[2].setFillColor(sf::Color::Red);
	_shapes[2].setSize(sf::Vector2f(200.0f, 50.0f));
	_shapes[2].setPosition(sf::Vector2f(winSize.x / 2 - _shapes[0].getGlobalBounds().width / 2, 210.0f));

	if (!_font.loadFromFile("Assets/Fonts/BernHC.ttf")) {
		std::cout << "Could not load font from file!" << std::endl;
	}

	_texts[0].setFont(_font);
	_texts[0].setCharacterSize(30);
	_texts[0].setFillColor(sf::Color::White);

	_texts[1].setFont(_font);
	_texts[1].setCharacterSize(30);
	_texts[1].setFillColor(sf::Color::White);
	_texts[1].setString("Options");
	_texts[1].setPosition(_shapes[1].getPosition().x, _shapes[1].getPosition().y + _shapes[1].getGlobalBounds().height / 2 - _texts[1].getGlobalBounds().height / 2);

	_texts[2].setFont(_font);
	_texts[2].setCharacterSize(30);
	_texts[2].setFillColor(sf::Color::White);
	_texts[2].setString("Quit to Desktop");
	_texts[2].setPosition(_shapes[2].getPosition().x, _shapes[2].getPosition().y + _shapes[2].getGlobalBounds().height / 2 - _texts[2].getGlobalBounds().height / 2);


	//SETUP BACKGROUND LAYERS

	//CLOUDS
	/*_backgroundTexture.push_back(sf::Texture());
	if (!_backgroundTexture[0].loadFromFile("Assets/Textures/Menu/Main/cloud_Background.png")) {
		std::cout << "Could not load font from file!" << std::endl;
	}
	_backgroundLayer.push_back(sf::Sprite(_backgroundTexture[0]));
	_backgroundLayer[0].setOrigin(_backgroundLayer[0].getGlobalBounds().width / 2, _backgroundLayer[0].getGlobalBounds().height / 2);
	_backgroundLayer[0].setPosition(sf::Vector2f(winSize.x / 2, winSize.y / 2));

	//GRASS MIDLAND
	/*_backgroundTexture.push_back(sf::Texture());
	if (!_backgroundTexture[1].loadFromFile("Assets/Textures/Menu/Main/grass_Midland.png")) {
		std::cout << "Could not load font from file!" << std::endl;
	}
	_backgroundLayer.push_back(sf::Sprite(_backgroundTexture[1]));
	_backgroundLayer[1].setOrigin(_backgroundLayer[1].getGlobalBounds().width / 2, _backgroundLayer[1].getGlobalBounds().height / 2);
	_backgroundLayer[1].setPosition(sf::Vector2f(winSize.x / 2, winSize.y / 2));*/

	EventManager* evManag = _stateManager->getShared()->_eventManager;
	evManag->addCallBack(StateType::MainMenu, "Mouse_Left", &State_MainMenu::mouseClicked, this);
}

void State_MainMenu::onDestroy() {
	EventManager* evManag = _stateManager->getShared()->_eventManager;
	evManag->removeCallback(StateType::MainMenu, "Mouse_Left");
}

void State_MainMenu::update(float deltaTime) {

}

void State_MainMenu::draw() {
	/*for (auto& layer : _backgroundLayer) {
		_stateManager->getShared()->_window->draw(layer.second);
	}*/
	_stateManager->getShared()->_window->draw(_backgroundSprite);
	_stateManager->getShared()->_window->draw(_introSprite);

	for (auto& shape : _shapes) {
		_stateManager->getShared()->_window->draw(shape);
	}
	for (auto& text : _texts) {
		_stateManager->getShared()->_window->draw(text);
	}
}

void State_MainMenu::activate() {
	if (_stateManager->hasState(StateType::Game)) {
		//already on stack
		_texts[0].setString("Resume");
		_shapes[0].setFillColor(sf::Color::Green);
	} else {
		//not yet on stack
		_texts[0].setString("Play");
		_shapes[0].setFillColor(sf::Color::Yellow);
	}
	_texts[0].setPosition(_shapes[0].getPosition().x, _shapes[0].getPosition().y + _shapes[0].getGlobalBounds().height / 2 - _shapes[0].getGlobalBounds().height / 2);
}

void State_MainMenu::deactivate() {}

void State_MainMenu::mouseClicked(EventDetails* test) {
	sf::Vector2f mousePos = _stateManager->getShared()->_window->getMouseWorldPos();
	if (_shapes[0].getGlobalBounds().contains(mousePos)) {
		_stateManager->requestSwitchTo(StateType::Game);
	}
	else if (_shapes[1].getGlobalBounds().contains(mousePos)) {
		_stateManager->requestSwitchTo(StateType::OptionsMenu);
	}
	else if (_shapes[2].getGlobalBounds().contains(mousePos)) {
		_stateManager->remove(StateType::MainMenu);
		_stateManager->getShared()->_window->close();
	}
}
