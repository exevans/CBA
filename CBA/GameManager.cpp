#include "GameManager.h"
#include "pugixml.hpp"
#include "Entity.h"
#include "LuaEntityHandle.h"
#include "DialogueManager.h"

std::string GameManager::_currentCell = "Null";
std::string GameManager::_newCell = "Null";
//game::State GameManager::_gameState = game::Uninitialized;
/*unsigned int GameManager::_triggerId = 0;
sf::View GameManager::_view(sf::FloatRect(0, 0, 800, 600));*/

/*unsigned int GameManager::_frames = 0;
sf::Clock GameManager::_frameRateClock;*/

//luabridge::lua_State* GameManager::L = luabridge::luaL_newstate();

/*void GameManager::startConversation(Entity* otherSpeaker)
{
	DialogueManager::_otherSpeaker = otherSpeaker;
	DialogueManager::_newConversation = true;
}*/

/*void GameManager::loadCell(std::string cellID)
{
	_newCell = cellID;
	_gameState = game::Loading;
}*/

/*void GameManager::setTriggerId(unsigned int triggerId)
{
	_triggerId = triggerId;
}*/

/*void GameManager::drawFrameRate()
{
	if (_frameRateClock.getElapsedTime().asMilliseconds() > 1000)
	{
		_frameRateClock.restart();
		std::cout << _frames << std::endl;
		_frames = 0;
	}
	else
		_frames++;
}*/

/*void GameManager::registerLua()
{
	luaL_openlibs(L);

	if (luaL_dofile(GameManager::L, "Assets/Scripts/Items.lua") == 0) {
		int rt = 0;
	}
	if (luaL_dofile(GameManager::L, "Assets/Scripts/Characters.lua") == 0) {
		int gdg = 0;
	}

	luabridge::getGlobalNamespace(L)
		.beginClass<sf::Vector2f>("Vector2f")
		.addConstructor<void(*)(float, float)>()
		.addData("x", &sf::Vector2f::x)
		.addData("y", &sf::Vector2f::y)
		.endClass()

		.beginClass<Entity>("Entity")
		.addConstructor <void(*) (void)>()
		.endClass()

		.beginClass<LuaEntityHandle>("LuaEntityHandle")
		.addConstructor <void(*) (Entity*)>()
		.addFunction("loadCell", &LuaEntityHandle::loadCell)

		.addFunction("hasTag", &LuaEntityHandle::hasTag)
		.addFunction("setHP", &LuaEntityHandle::setHp)
		.addFunction("addItemToInventory", &LuaEntityHandle::addItemToInventory)
		.addFunction("addContentsToInventory", &LuaEntityHandle::addContentsToInventory)
		.addFunction("isInventoryFull", &LuaEntityHandle::isInventoryFull)
		.addFunction("isInventoryEmpty", &LuaEntityHandle::isInventoryEmpty)
		.addFunction("removeItemFromInventory", &LuaEntityHandle::removeItemFromInventory)
		.addFunction("heal", &LuaEntityHandle::heal)
		.addFunction("damage", &LuaEntityHandle::damage)
		.addFunction("destroy", &LuaEntityHandle::setInactive)
		.addFunction("spawnChild", &LuaEntityHandle::spawnChild)
		.addFunction("moveTowardsPosition", &LuaEntityHandle::moveTowardsPosition)
		.addFunction("moveOutOfIntersection", &LuaEntityHandle::moveOutOfIntersection)
		.addFunction("setRouteByNavMesh", &LuaEntityHandle::setRouteByNavMesh)
		.addFunction("say", &LuaEntityHandle::outputText)
		.addFunction("getDistanceToPos", &LuaEntityHandle::getDistanceToPosition)
		.addFunction("setEnemy", &LuaEntityHandle::setEnemyEntity)
		.addFunction("beginConversation", &LuaEntityHandle::startDialogue)
		.addFunction("setScriptState", &LuaEntityHandle::setScriptState)
		.addFunction("getScriptState", &LuaEntityHandle::getScriptState)
		.addFunction("setAnimation", &LuaEntityHandle::setAnimation)
		.addFunction("setCollision", &LuaEntityHandle::setCollision)

		.addFunction("equipWeapon", &LuaEntityHandle::equipWeapon)
		.addFunction("patrol", &LuaEntityHandle::patrol)
		.addFunction("followRoute", &LuaEntityHandle::followRoute)
		.addFunction("getEnemy", &LuaEntityHandle::getEnemy)
		.addFunction("hasEnemy", &LuaEntityHandle::hasEnemy)
		.addFunction("wander", &LuaEntityHandle::wander)
		.addFunction("flee", &LuaEntityHandle::flee)
		.addFunction("stopMotion", &LuaEntityHandle::stopMotion)
		.addFunction("attackEnemy", &LuaEntityHandle::attackEnemy)

		.addFunction("timerTicked", &LuaEntityHandle::timerTicked)

		.addFunction("notifyObservers", &LuaEntityHandle::notify)

		.addProperty("position", &LuaEntityHandle::getPosition, &LuaEntityHandle::setPosition)
		.addProperty("targetPosition", &LuaEntityHandle::getTargetPosition, &LuaEntityHandle::setTargetPosition)
		.addProperty("guardPosition", &LuaEntityHandle::getGuardPosition, &LuaEntityHandle::setGuardPosition)
		.endClass();
}*/