#ifndef ENGINE_H
#define	ENGINE_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "TextManager.h"
#include "SharedContext.h"
#include "Window.h"
#include "ObjectFactory.h"
#include "MapManager.h"
#include "DialogueManager.h"
#include "LuaEntityHandle.h"
#include "LightManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "QuestManager.h"
#include "StateManager.h"
#include "LuaManager.h"
#include "NavMeshManager.h"

class Engine
{
public:
	Engine();
	~Engine();
	void run();

private:
	void init();
	void mainLoop();
	void update(const float);
	void lateUpdate();
	void saveGame();
	void loadGame();
	//void updateEditor(float);
	void render();
	void MaintainAspectRatio();

	sf::Vector2u currentAspect;
	float aspectRatio;

	sf::Clock _clock;

	Window _window2;

	SharedContext _context;
	StateManager _stateManager;
	SystemManager _systemManager;
	EntityManager _entityManager;
	LuaManager _luaManager;
	NavMeshManager _navMeshManager;
	MapManager _mapManager;
	LightManager _lightManager;
	TextManager _textManager;
	//KeyBinder _inputManager;
	//QuestManager _questManager;
	//DialogueManager _dialogueManager;
};

#endif