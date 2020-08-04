#include "State_Game.h"
#include "StateManager.h"
#include "SharedContext.h"
#include "EventManager.h"
#include "Window.h"
#include "InputSystem.h"
#include "MovementSystem.h"
#include "GameLogic.h"
#include "ObjectFactory.h"
#include "GraphicsSystem.h"
#include "AnimationSystem.h"
#include "CollisionSystem.h"
#include "Direction.h"
#include "LightManager.h"
#include "EntityManager.h"
#include "InteractSystem.h"
#include "ScriptStateManagerSystem.h"
#include "LuaWorldHandle.h"
#include "TextManager.h"
#include "LuaManager.h"
#include "Entity.h"
#include "Component.h"

State_Game::State_Game(StateManager* sManager) : State(sManager), _luaWorldHandle(_stateManager->getShared()) { 
	_stateManager->getShared()->_luaManager->getLuaState()["World"] = _luaWorldHandle; 
	loadLevel("TestMap_128_128"); 
}

void State_Game::onCreate() {
	EventManager* evManag = _stateManager->getShared()->_eventManager;
	evManag->addCallBack(StateType::Game, "Key_Escape", &State_Game::mainMenu, this);
	evManag->addCallBack(StateType::Game, "Key_P", &State_Game::pause, this);
	evManag->addCallBack(StateType::Game, "Player_Interact", &State_Game::playerInteract, this);
	evManag->addCallBack(StateType::Game, "Player_MoveUp", &State_Game::movePlayer, this);
	evManag->addCallBack(StateType::Game, "Player_MoveLeft", &State_Game::movePlayer, this);
	evManag->addCallBack(StateType::Game, "Player_MoveRight", &State_Game::movePlayer, this);
	evManag->addCallBack(StateType::Game, "Player_MoveDown", &State_Game::movePlayer, this);
	evManag->addCallBack(StateType::Game, "Mouse_Left", &State_Game::playerPrimaryFire, this);
	evManag->addCallBack(StateType::Game, "Key_Console", &State_Game::toggleConsole, this);

	evManag->addCallBack(StateType::Game, "Joy_P", &State_Game::pause, this);

	_view.reset(sf::FloatRect(0, 0, _stateManager->getShared()->_window->getWinSize().x, _stateManager->getShared()->_window->getWinSize().y));
	_view.zoom(0.8);
	//_dialogueManager.addObserver(_questManager);	//questmanager observes dialoigue
}

void State_Game::onDestroy() {
	EventManager* evManag = _stateManager->getShared()->_eventManager;
	evManag->removeCallback(StateType::Game, "Key_Escape");
	evManag->removeCallback(StateType::Game, "Key_P");
	evManag->removeCallback(StateType::Game, "Player_MoveUp");
	evManag->removeCallback(StateType::Game, "Player_MoveLeft");
	evManag->removeCallback(StateType::Game, "Player_MoveRight");
	evManag->removeCallback(StateType::Game, "Player_MoveDown");
	evManag->removeCallback(StateType::Game, "Player_Interact");
	evManag->removeCallback(StateType::Game, "Mouse_Left");
	evManag->removeCallback(StateType::Game, "Key_Console");

	evManag->removeCallback(StateType::Game, "Joy_P");
}

void State_Game::update(float deltaTime) {
	//check if the level should be loading to be switched
	checkForLoadRequests();

	_stateManager->getShared()->_systemManager->update(deltaTime);
	_stateManager->getShared()->_textManager->update(deltaTime);

	//update lighting manager
	_stateManager->getShared()->_lightManager->update(deltaTime);

	//to be switched
	checkForDialogueRequests();
	updateCamera();
}

void State_Game::draw() {
	_stateManager->getShared()->_window->draw(*_stateManager->getShared()->_mapManager);	//draw map
	//_stateManager->getShared()->_navMeshManager->draw(_stateManager->getShared()->_window);	//draw navmesh (for debugging)
	_stateManager->getShared()->_systemManager->draw(_stateManager->getShared()->_window);	//draw any systems(entites)
	_stateManager->getShared()->_lightManager->draw(_stateManager->getShared()->_window);	//draw lighting
	_stateManager->getShared()->_textManager->draw(_stateManager->getShared()->_window);	//draw text overlays
}

void State_Game::activate() {}

void State_Game::deactivate() {}

void State_Game::mainMenu(EventDetails* details) {
	_stateManager->requestSwitchTo(StateType::MainMenu);
}

void State_Game::pause(EventDetails* details) {
	_stateManager->requestSwitchTo(StateType::Paused);
}

void State_Game::checkForLoadRequests() {
	std::string newLevelPath;
	if (! _stateManager->getShared()->_entityManager->loadingLevel(newLevelPath)) { return; }

	//check the file is a valid name
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(std::string("Assets/Maps/" + newLevelPath + ".tmx").c_str());
	if (!result)
	{
		std::cout << "XML [" << newLevelPath << "] parsed with errors" << std::endl;
		std::cout << "Error description: " << result.description() << "\n";
		return;
	}

	//load the level
	loadLevel(newLevelPath);	
}

void State_Game::checkForDialogueRequests() {
	if (!_stateManager->getShared()->_entityManager->hasComponent(_player, ComponentType::Dialogue)) { return; }
	if (_stateManager->getShared()->_entityManager->getComponent<DialogueComponent>(_player, ComponentType::Dialogue)->_inConversation) {
		_stateManager->requestSwitchTo(StateType::Dialogue);
	}
}

void State_Game::activatePlayer(EventDetails* details) {
	_stateManager->getShared()->_systemManager->activatePlayer();
}

void State_Game::movePlayer(EventDetails* details) {
	Message mess = Message(MessageType(EntityMessage::Move));
	if (details->_name == "Player_MoveUp") {
		mess._int = (int)Direction::Up;
	}
	if (details->_name == "Player_MoveDown") {
		mess._int = (int)Direction::Down;
	}
	if (details->_name == "Player_MoveLeft") {
		mess._int = (int)Direction::Left;
	}
	if (details->_name == "Player_MoveRight") {
		mess._int = (int)Direction::Right;
	}

	mess._receiver = _player;
	_stateManager->getShared()->_systemManager->getMessageHandler()->dispatch(mess);
}

void State_Game::playerInteract(EventDetails* details) {
	Message mess = Message(MessageType(EntityMessage::Interact));
	mess._receiver = _player;
	_stateManager->getShared()->_systemManager->getMessageHandler()->dispatch(mess);
}

void State_Game::playerPrimaryFire(EventDetails* details) {
	Message mess = Message(MessageType(EntityMessage::PrimaryFire));
	mess._receiver = _player;
	_stateManager->getShared()->_systemManager->getMessageHandler()->dispatch(mess);
}

void State_Game::toggleConsole(EventDetails* details) {
	bool inConsole = true;
	
	while (inConsole) {
		std::string consoleStr;
		std::getline(std::cin, consoleStr);

		if (consoleStr == "")
			inConsole = false;
		else
			_stateManager->getShared()->_luaManager->getLuaState().do_string(consoleStr);
	}
}

void State_Game::saveLevel()
{
	//dynamic_cast<ObjectFactorySystem*>(_system[0].get())->saveLevel();
	//_questManager.saveData();
	//GameManager::_gameState = game::Playing;
}

void State_Game::loadLevel(const std::string& path)
{
	_stateManager->getShared()->_textManager->purge();
	_stateManager->getShared()->_luaManager->purge();
	//_questManager.loadData();
	_stateManager->getShared()->_mapManager->loadMap("Assets/Maps/" + path + ".tmx");
	_stateManager->getShared()->_lightManager->loadLightMap("Assets/Maps/" + path + ".tmx");
	_stateManager->getShared()->_entityManager->loadLevel("Assets/Maps/" + path + ".tmx");
	_stateManager->getShared()->_systemManager->loadLevel(path);
	_stateManager->getShared()->_navMeshManager->loadMap("Assets/Maps/" + path + ".tmx");
	/*dynamic_cast<CollisionSystem*>(_system[4].get())->loadCollisionMap("Assets/Maps/" + GameManager::_newCell + ".tmx");
	dynamic_cast<NavMeshSystem*>(_system[2].get())->loadMap("Assets/Maps/" + GameManager::_newCell + ".tmx");
	for (auto& it : _system)
		it->removeAllComponents();

	if (GameManager::_currentCell != "Null")
		this->saveGame();

	dynamic_cast<ObjectFactorySystem*>(_system[0].get())->loadLevel("Assets/Maps/" + GameManager::_newCell + ".tmx");*/
	//GameManager::_currentCell = GameManager::_newCell;

	//GameManager::_gameState = game::Playing;
}

void State_Game::updateCamera() {
	SharedContext* shared = _stateManager->getShared();
	if (_stateManager->getShared()->_entityManager->hasComponent(_player, ComponentType::Position)) {
		PositionComponent* posComp = _stateManager->getShared()->_entityManager->getComponent<PositionComponent>(_player, ComponentType::Position);
		_view.setCenter(posComp->_position);
		_stateManager->getShared()->_window->getRenderWindow()->setView(_view);
	}

	sf::FloatRect viewSpace = shared->_window->getViewSpace();
	if (viewSpace.left <= 0) {
		_view.setCenter(viewSpace.width/2, _view.getCenter().y);
		_stateManager->getShared()->_window->getRenderWindow()->setView(_view);
	}
	if (viewSpace.top <= 0) {
		_view.setCenter(_view.getCenter().x, viewSpace.height/2);
		_stateManager->getShared()->_window->getRenderWindow()->setView(_view);
	}

	

}
