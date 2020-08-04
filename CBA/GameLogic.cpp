#include "GameLogic.h"
#include "SystemManager.h"
#include "Component.h"
#include "Entity.h"
#include <iostream>

LogicSystem::LogicSystem(SystemManager* sysManager)
	:System(sysManager, SystemType::Logic)
{
	//addRequiredComponentType(ComponentType::Health);
}

void LogicSystem::update(float deltaTime)
{
	EntityManager* entManager = _systemManager->getEntityManager();
	for (auto& ent : _entities) {
		if (!entManager->getEntityInfo(ent).active) {
			_systemManager->addEvent(ent, (int)EntityEvent::Despawned);
		}
	}
}

void LogicSystem::handleEvent(const EntityID& ent, const EntityEvent& entEvent) {

}
