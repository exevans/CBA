#include "LuaManager.h"
#include "EntityManager.h"
#include <iostream>
#include "LuaEntityHandle.h"
#include "LuaWorldHandle.h"

LuaManager::LuaManager() {
	registerLua();
}

LuaManager::~LuaManager() {
	purge();
	/*if (_L) {
		lua_close(_L);
	}*/
}

void LuaManager::setEntityManager(EntityManager* entManag) {
	_entityManager = entManag;
}

sol::state& LuaManager::getLuaState() {
	return _L;
}

void LuaManager::registerLua() {
	_L.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::string, sol::lib::io);

	_L.script_file("Assets/Scripts/Entity.lua");
	_L.script_file("Assets/Scripts/World.lua");

	_L.new_usertype<sf::Vector2f>("Vector2f",
		sol::constructors<sf::Vector2f(), sf::Vector2f(float, float)>(),
		"x", &sf::Vector2f::x,
		"y", &sf::Vector2f::y
	);

	_L.new_usertype<LuaWorldHandle>("World",
		"loadLevel", &LuaWorldHandle::loadLevel,
		"reloadLevel", &LuaWorldHandle::reloadLevel,
		"getIdFromFileId", &LuaWorldHandle::getIdFromFile,
		"spawn", &LuaWorldHandle::spawn,
		"beginConversation", &LuaWorldHandle::startDialogue,
		"displayText", &LuaWorldHandle::addTextMessage,
		"removeText", &LuaWorldHandle::removeTextMessage,
		"getDistanceBetweenPositions", &LuaWorldHandle::getDistanceBetweenPositions,
		"test", &LuaWorldHandle::test
	);

	_L.new_usertype<LuaEntityHandle>("Entity",
		"getId", &LuaEntityHandle::getId,
		//"loadCell", &LuaEntityHandle::loadCell,

		"hasTag", &LuaEntityHandle::hasTag,
		"setHP", &LuaEntityHandle::setHp,
		"addItemToInventory", &LuaEntityHandle::addItemToInventory,
		"addContentsToInventory", &LuaEntityHandle::addContentsToInventory,
		"isInventoryFull", &LuaEntityHandle::isInventoryFull,
		"isInventoryEmpty", &LuaEntityHandle::isInventoryEmpty,
		"removeItemFromInventory", &LuaEntityHandle::removeItemFromInventory,
		"heal", &LuaEntityHandle::heal,
		"damage", &LuaEntityHandle::damage,
		"destroy", &LuaEntityHandle::setInactive,
		//"spawn", &LuaEntityHandle::spawn,
		//"spawnChild", &LuaEntityHandle::spawnChild,
		"moveTowardsPosition", &LuaEntityHandle::moveTowardsPosition,
		"moveOutOfIntersection", &LuaEntityHandle::moveOutOfIntersection,
		"setRouteByNavMesh", &LuaEntityHandle::setRouteByNavMesh,
		"say", &LuaEntityHandle::outputText,
		"getDistanceToPos", &LuaEntityHandle::getDistanceToPosition,
		"setEnemy", &LuaEntityHandle::setEnemyEntity,
		//"beginConversation", &LuaEntityHandle::startDialogue,

		"addScheduleTask", sol::overload(&LuaEntityHandle::addScheduleTask, &LuaEntityHandle::addScheduleTask2f, &LuaEntityHandle::addScheduleTaskStr, &LuaEntityHandle::addScheduleTaskInt),
		"clearSchedule", &LuaEntityHandle::clearSchedule,

		"startTimer", &LuaEntityHandle::startTimer,
		"stopTimer", &LuaEntityHandle::stopTimer,

		"setControllable", &LuaEntityHandle::setControllable,
		"noclip", &LuaEntityHandle::toggleNoclip,

		"setInDirToEnt", &LuaEntityHandle::setInMovementToEnt,

		"setScriptState", &LuaEntityHandle::setScriptState,
		"getScriptState", &LuaEntityHandle::getScriptState,
		"setAnimation", &LuaEntityHandle::setAnimation,
		"setCollision", &LuaEntityHandle::setCollision,

		"setPosition", &LuaEntityHandle::setPosition,
		"equipWeapon", &LuaEntityHandle::equipWeapon,
		"patrol", &LuaEntityHandle::patrol,
		"followRoute", &LuaEntityHandle::followRoute,
		"getEnemy", &LuaEntityHandle::getEnemy,
		"hasEnemy", &LuaEntityHandle::hasEnemy,
		"wander", &LuaEntityHandle::wander,
		"flee", &LuaEntityHandle::flee,
		"stopMotion", &LuaEntityHandle::stopMotion,
		"attackEnemy", &LuaEntityHandle::attackEnemy,

		"timerTicked", &LuaEntityHandle::timerTicked,
		"notifyObservers", &LuaEntityHandle::notifyObservers,

		"setTargetPosition", &LuaEntityHandle::setTargetPosition,
		"getTargetPosition", &LuaEntityHandle::getTargetPosition,
		"setPosition", &LuaEntityHandle::setPosition,
		"getPosition", &LuaEntityHandle::getPosition,
		"position", sol::property(&LuaEntityHandle::getPosition, &LuaEntityHandle::setPosition)
	);

	//_L = sol::state;	//creates new lua state
	//_L = luaL_newstate();
	//luaL_openlibs(_L);	//opens libaries
	
	/*luabridge::getGlobalNamespace(_L)
		.beginClass<sf::Vector2f>("Vector2f")
		.addConstructor<void(*)(float, float)>()
		.addData("x", &sf::Vector2f::x)
		.addData("y", &sf::Vector2f::y)
		.endClass()

		.beginClass<EntityManager>("EntityManager")
		.addConstructor <void(*) (void)>()
		.endClass()

		.beginClass<LuaEntityHandle>("LuaEntityHandle")
		.addConstructor <void(*) (EntityManager*, unsigned int)>()
		.addFunction("getId", &LuaEntityHandle::getId)
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
		.endClass();*/
}

const sol::table& LuaManager::getEntityLuaRef(const EntityID& entity) {
	auto itr = _entityScripts.find(entity);
	return itr->second;
}

void LuaManager::loadEntityLua(const EntityID& entity, const std::string& scriptPath, const std::string& scriptTable) {
	//can't already have entity loaded
	auto itr = _entityScripts.find(entity);
	if (itr != _entityScripts.end()) { return; }

	_L.script_file(scriptPath);

	_entityScripts.emplace(entity, _L[scriptTable]);
}

void LuaManager::purgeScripts() {
	_entityScripts.clear();
	//purgeObservers();
}

bool LuaManager::addLuaEvent(const EntityID& ent, const std::string& eventToAdd) {
	auto itr = _luaEntityObservers.find(ent);
	//if entity is unknown add it to list
	if (itr == _luaEntityObservers.end()) {
		_luaEntityObservers.emplace(ent, LuaEventObservers());
	}

	itr = _luaEntityObservers.find(ent);
	itr->second.emplace(eventToAdd, LuaEntityCallbackContainer());

	return true;
}

bool LuaManager::subscribeToLuaEvent(const EntityID& entToSubTo, const EntityID& thisEnt , const std::string& eventToSubscribeTo, const std::string& luaCallback) {
	//ensure other entity exits 
	if (!_entityManager->hasEntity(entToSubTo)) { return false; }

	//if event isnt yet being monitored make it so
	auto itr = _luaEntityObservers.find(entToSubTo);
	if (itr == _luaEntityObservers.end()) { 
		addLuaEvent(entToSubTo, eventToSubscribeTo);
		itr = _luaEntityObservers.find(entToSubTo);
	}

	//add iterator to the event to sub to
	auto itr2 = itr->second.find(eventToSubscribeTo);
	if (itr2 == itr->second.end()) { return false; }

	//add the callback infor to the event
	itr2->second.emplace_back(thisEnt, luaCallback);

	return true;
}

bool LuaManager::unsubscribeFromLuaEvent(const EntityID& entToSubTo, const EntityID& thisEnt, const std::string& eventToSubscribeTo) {
	auto itr = _luaEntityObservers.find(entToSubTo);
	if (itr == _luaEntityObservers.end()) { return false; }

	auto itr2 = itr->second.find(eventToSubscribeTo);
	if (itr2 == itr->second.end()) { return false; }

	for (auto& callbackItr = itr2->second.begin(); callbackItr != itr2->second.end(); ) {
		if (callbackItr->first == thisEnt) {
			itr2->second.erase(callbackItr);
		}
		else {
			itr2++;
		}
	}

	return true;
}

void LuaManager::notifyObservers(const EntityID& ent, const std::string& triggeredEvent) {
	auto itr = _luaEntityObservers.find(ent);
	if (itr == _luaEntityObservers.end()) { return; }

	auto itr2 = itr->second.find(triggeredEvent);
	if (itr2 == itr->second.end()) { return; }

	//call lua callback
	for (auto reciever : itr2->second) {
		sol::function callback = _L[_entityManager->getEntityInfo(reciever.first).scriptTable][reciever.second];
		if (callback.valid()) {
			callback(reciever.first);
		}
	}
}

void LuaManager::purgeObservers() {
	_luaEntityObservers.clear();
}

void LuaManager::purge() {
	purgeScripts();
	purgeObservers();
}

void LuaManager::removeEntityFromObservers(const EntityID& ent) {
	//TODO
	auto itr = _luaEntityObservers.find(ent);
	if (itr != _luaEntityObservers.end()) { 
		_luaEntityObservers.erase(itr);
	}

	/*for (auto entitys : _luaEntityObservers) {
		for (auto entityEvents : entitys.second) {
			auto itr2 = entityEvents.second.begin();
			while (itr2 != entityEvents.second.end()) {
				if (itr2->first == ent) {
					entityEvents.second.erase(itr2);
				}
				else {
					itr2++;
				}
			}
		}
	}*/
}

void LuaManager::removeEntity(const EntityID& ent) {
	auto& itr = _entityScripts.find(ent);
	if (itr == _entityScripts.end()) { return; }

	_entityScripts.erase(itr);
	removeEntityFromObservers(ent);
}