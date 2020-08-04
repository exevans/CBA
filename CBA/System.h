#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <typeindex>
#include <SFML/Graphics.hpp>
#include "Subject.h"
#include "Observer.h"
#include "SystemType.h"
#include "EntityManager.h"

//#include <LuaBridge.h>
extern "C"
{
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

using EntityList = std::vector<EntityID>;

class SystemManager;
class Component;
class Entity;

class System : public Observer, public Subject
{
public:
	bool hasEntity(const EntityID&);
	bool addEntity(const EntityID&);
	bool removeEntity(const EntityID&);
	void purge();

	//void removeAllComponents();
	//for when a entity is tagged as removed
	//void checkForRemovedComponents();
	virtual void update(float) = 0;
	virtual void handleEvent(const EntityID&, const EntityEvent&)  = 0;

	_Uint32t getSystemLockID();
	SystemType getSystemType() const;

	virtual void onNotify(Entity*, std::string) override;
protected:
	System(SystemManager*, const SystemType&);

	void addRequiredComponentType(const ComponentType&);

	SystemManager* _systemManager;
	EntityList _entities;
private:
	//system lock of compeents required for valid entity
	_Uint32t _systemLockID;
	//type of system
	SystemType _systemType = SystemType::Logic;
};

#endif