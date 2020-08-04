#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <memory>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "MapManager.h"
//#include "EventManager.h"
#include "Window.h"

//#include <LuaBridge.h>

class Entity;

extern "C"
{
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

namespace game
{
	enum State { Uninitialized, Loading, Saving, Playing, Exiting, InMenu, Editing };
}

class GameManager
{
public:
	static std::string _currentCell;
	static std::string _newCell;
	//static game::State _gameState;
	//static unsigned int _triggerId;

	//static sf::View _view;

	//static void startConversation(Entity*);

	//static void setTriggerId(unsigned int);
	//static void loadCell(std::string);
	//static void drawFrameRate();

	//static luabridge::lua_State* L;
	//static void registerLua();

private:
	/*static sf::Clock _frameRateClock;
	static unsigned int _frames;*/
};

#endif