#include "LightingSystem.h"

#include "Component.h"
#include "EntityMessage.h"
#include "LightManager.h"
#include "Entity.h"
#include <iostream>

LightingSystem::LightingSystem(SystemManager* sysManager)
	:System(sysManager, SystemType::Lighting)
{
	addRequiredComponentType(ComponentType::Lighting);
}

void LightingSystem::update(float deltaTime)
{
	EntityManager* entManager = _systemManager->getEntityManager();
	for (auto ent : _entities) {
		PositionComponent* posComp = entManager->getComponent<PositionComponent>(ent, ComponentType::Position);
		LightingComponent* lightComp = entManager->getComponent<LightingComponent>(ent, ComponentType::Lighting);
		lightComp->_positionRoot = posComp->_position;
		_systemManager->getShared()->_lightManager->updateDynamicLightPosition(ent, lightComp->_positionRoot);
	}
}

void LightingSystem::handleEvent(const EntityID& ent, const EntityEvent& entEvent) {

}

void LightingSystem::onNotify(const Message& message) {

}
