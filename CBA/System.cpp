#include "System.h"
#include "Component.h"
#include "LuaEntityHandle.h"
#include "GraphicsSystem.h"
#include "SystemManager.h"
#include "Entity.h"
#include "Engine.h"
#include <algorithm>
#include <iostream>

System::System(SystemManager* sysManager, const SystemType& systemType) : _systemManager(sysManager), _systemType(systemType), _systemLockID(0)
{

}

bool System::hasEntity(const EntityID& entity) {
	auto itr = std::find(_entities.begin(), _entities.end(), entity);
	if (itr != _entities.end()) {
		return true;
	}

	return false;
}

bool System::addEntity(const EntityID& entity) {
	if (hasEntity(entity)) {
		return false;
	}
	_entities.emplace_back(entity);
	return true;
}

bool System::removeEntity(const EntityID& entity) {
	if (!hasEntity(entity)) {
		return false;
	}
	auto itr = std::find(_entities.begin(), _entities.end(), entity);
	_entities.erase(itr);
	return true;
}

void System::purge() {
	_entities.clear();
}

/*void System::removeAllComponents()
{
	for (std::map<std::type_index, std::map<unsigned int, Component*>>::iterator it = _components.begin(); it != _components.end(); ++it)
	{
		//for all compents of a type
		std::map<unsigned int, Component*>::iterator ot = it->second.begin();
		while (ot != it->second.end())
		{
			//remove the compenent
			ot = it->second.erase(ot);
		}
	}
}*/

/*void System::checkForRemovedComponents()
{
	//for all components in system check that they are active if not remove them
	for (std::map<std::type_index, std::map<unsigned int, Component*>>::iterator it = _components.begin(); it != _components.end(); ++it)
	{
		std::map<unsigned int, Component*>::iterator ot = it->second.begin();
		while (ot != it->second.end())
		{
			if (!ot->second->isActive())	//not active
				ot = it->second.erase(ot);
			else
				++ot;
		}
	}
}*/

void System::addRequiredComponentType(const ComponentType& comType) {
	_systemLockID |= (1 << unsigned int(comType));
}

_Uint32t System::getSystemLockID()
{
	return _systemLockID;
}

SystemType System::getSystemType() const
{
	return _systemType;
}

void System::onNotify(Entity* entity, std::string eventType)
{
	
}