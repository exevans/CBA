#ifndef LUAENTHAND_H
#define LUAENTHAND_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

extern "C"
{
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

using EntityID = unsigned int;

class EntityManager;

class LuaEntityHandle
{
public:
	LuaEntityHandle(EntityManager*, const EntityID);
	EntityID getId();

	//Schedule stuff
	void addScheduleTask(const std::string&);
	void addScheduleTaskInt(const std::string&, int);
	void addScheduleTaskStr(const std::string&, std::string&);
	void addScheduleTask2f(const std::string&, sf::Vector2f&);
	void clearSchedule();

	//timers
	void startTimer(const std::string&);
	void stopTimer(const std::string&);

	void setControllable(bool);
	void toggleNoclip();

	void setInMovementToEnt(const unsigned int);

	bool hasTag(std::string) const;
	void setHp(const unsigned int);
	void addItemToInventory(LuaEntityHandle*);
	void addContentsToInventory(LuaEntityHandle*);
	bool isInventoryFull() const;
	bool isInventoryEmpty() const;
	void removeItemFromInventory(LuaEntityHandle*);
	void damage(unsigned int);
	void heal(unsigned int);
	//LuaEntityHandle spawn(std::string&, std::string&);
	//void spawnChild(std::string);
	void moveTowardsPosition(sf::Vector2f);
	void moveOutOfIntersection(const EntityID&);
	void setRouteByNavMesh(sf::Vector2f);
	void outputText(std::string);
	float getDistanceToPosition(sf::Vector2f);
	float getDistanceBetweenPositions(sf::Vector2f, sf::Vector2f);
	void setEnemyEntity(LuaEntityHandle*);
	//void startDialogue(LuaEntityHandle*);
	void setInactive();
	void setScriptState(std::string);
	std::string getScriptState();
	void setAnimation(std::string, bool, bool);
	void setCollision(sf::Vector2f, int, int, int, int);

	void activateItem();
	void patrol();
	void equipWeapon();
	LuaEntityHandle getEnemy();
	bool hasEnemy();
	void wander();
	void followRoute();
	void flee();
	void randomizeTargetPosition();
	void stopMotion();
	void attackEnemy();

	bool timerTicked(std::string);

	void notifyObservers(const std::string&);

	sf::Vector2f getTargetPosition() const;
	void setTargetPosition(sf::Vector2f);
	sf::Vector2f getGuardPosition() const;
	void setGuardPosition(sf::Vector2f);
	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f);

private:
	EntityID _e;
	EntityManager* _entityManager;
};

#endif