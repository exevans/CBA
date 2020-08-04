#include "Engine.h"
#include "System.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"
#include "MovementSystem.h"
#include "GameLogic.h"
#include "AnimationSystem.h"
#include "CollisionSystem.h"
#include "DialogueSystem.h"
#include "InteractSystem.h"
#include "ScriptStateManagerSystem.h"
#include "Entity.h"
#include "Component.h"

Engine::Engine() : _window2("Elysium Plaines", sf::Vector2u(1920, 1080))
	, _systemManager(&_context)
	, _stateManager(&_context)
	, _entityManager(&_systemManager, &_luaManager, &_navMeshManager)
	, _textManager(&_entityManager)
{
	_systemManager.setEntityManager(&_entityManager);
	_navMeshManager.setEntityManager(&_entityManager);

	//apply shared context values
	_context._window = &_window2;
	_context._eventManager = _window2.getEventManager();
	_context._systemManager = &_systemManager;
	_context._entityManager = &_entityManager;
	_context._luaManager = &_luaManager;
	_context._mapManager = &_mapManager;
	_context._textManager = &_textManager;
	_context._navMeshManager = &_navMeshManager;
	_context._lightManager = &_lightManager;

	_stateManager.requestSwitchTo(StateType::Intro);
}

Engine::~Engine() {
	std::cout << "starting to destroy engine" << std::endl;
}

void Engine::run()
{
	while (!_window2.isDone())
	{
		this->mainLoop();
	}
}

void Engine::init()
{
	
}

void Engine::mainLoop()
{
	float deltaTime = _clock.restart().asSeconds();

	this->update(deltaTime);
	this->render();
	this->lateUpdate();
}

void Engine::update(const float deltaTime)
{
	_window2.update();
	_stateManager.update(deltaTime);
}

void Engine::lateUpdate() {
	_stateManager.processRequests();
}

void Engine::saveGame()
{
	/*dynamic_cast<ObjectFactorySystem*>(_system[0].get())->saveLevel();
	_questManager.saveData();
	GameManager::_gameState = game::Playing;*/
}

void Engine::loadGame()
{
	/*_questManager.loadData();
	_map.loadMap("Assets/Maps/" + GameManager::_newCell + ".tmx");
	dynamic_cast<CollisionSystem*>(_system[4].get())->loadCollisionMap("Assets/Maps/" + GameManager::_newCell + ".tmx");
	dynamic_cast<NavMeshSystem*>(_system[2].get())->loadMap("Assets/Maps/" + GameManager::_newCell + ".tmx");
	for (auto& it : _system)
		it->removeAllComponents();

	if (GameManager::_currentCell != "Null")
		this->saveGame();

	dynamic_cast<ObjectFactorySystem*>(_system[0].get())->loadLevel("Assets/Maps/" + GameManager::_newCell + ".tmx");
	GameManager::_currentCell = GameManager::_newCell;

	GameManager::_gameState = game::Playing;  */
}

/*void Engine::updateEditor(float deltaTime)
{
	if (KeyBinder::isEventPending("Activate"))
		std::cout << "grdgdrg ";
}*/

void Engine::render()
{
	//for rendering to win 2
	_window2.beginDraw();
	//draw everything in state
	_stateManager.draw();
	//display to screen
	_window2.endDraw();
}

void Engine::MaintainAspectRatio()
{
	sf::Vector2u newAspect = _window2.getWinSize();

	if (newAspect.x != currentAspect.x)
	{
		//width changed, adjust height 
		currentAspect.x = newAspect.x;
		currentAspect.y = (currentAspect.x / aspectRatio);
	}
	else if (newAspect.y != currentAspect.y)
	{
		//height changed, adjust width
		currentAspect.y = newAspect.y;
		currentAspect.x = currentAspect.y * aspectRatio;
	}

	//_window.setSize(currentAspect);
	std::cout << "aspect: " << aspectRatio << " new window size: " << _window2.getWinSize().x << " x " << _window2.getWinSize().y << std::endl;
}