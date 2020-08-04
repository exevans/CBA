#ifndef LUA_WORLD_HANDLE_H
#define LUA_WORLD_HANDLE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

#include "SharedContext.h"

extern "C"
{
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

using EntityID = unsigned int;

class LuaWorldHandle {
public:
	LuaWorldHandle(SharedContext*);
	void loadLevel(const std::string&);
	void reloadLevel();
	int getIdFromFile(int);
	int spawn(const std::string&, const std::string&);
	void startDialogue(const EntityID&);
	void addTextMessage(const EntityID&, const std::string&, const sf::Vector2f&, const float);
	void removeTextMessage(const EntityID&);

	float getDistanceBetweenPositions(sf::Vector2f, sf::Vector2f);

	void test();
private:
	SharedContext* _sharedCon;
};

#endif