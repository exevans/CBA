#include "InteractSystem.h"
#include "Component.h"
#include "Entity.h"
#include "SystemManager.h"
#include "LuaManager.h"

InteractSystem::InteractSystem(SystemManager* sysManager)
	: System(sysManager, SystemType::Interact)
{
	addRequiredComponentType(ComponentType::Position);
	addRequiredComponentType(ComponentType::Interact);
}

void InteractSystem::update(float deltaTime)
{
	/*EntityManager* entManager = _systemManager->getEntityManager();
	for (auto& ent : _entities) {
		InteractComponent* interactComp = entManager->getComponent<InteractComponent>(ent, ComponentType::Interact);
		PositionComponent* posComp = entManager->getComponent<PositionComponent>(ent, ComponentType::Position);

		//if (it->second->getOwnerEntity()->hasComponent(std::type_index(typeid(InputComponent))))	//PLAYER
		//{
			interactComp->_interactRect.left = posComp->_position.x + interactComp->_localInteractRectOffset.x;	//set bounding box position
			interactComp->_interactRect.top = posComp->_position.y + interactComp->_localInteractRectOffset.y;

			for (std::map<unsigned int, Component*>::iterator ot = interactComps.begin(); ot != interactComps.end(); ++ot)
			{
				auto interactComp2 = this->get<InteractComponent>(ot->first);
				auto posComp2 = this->get<PositionComponent>(ot->first);

				interactComp2->_interactRect.left = posComp2->_position.x + interactComp2->_localInteractRectOffset.x;	//set bounding box posotion
				interactComp2->_interactRect.top = posComp2->_position.y + interactComp2->_localInteractRectOffset.y;

				sf::FloatRect ent1Rect = (interactComp->_interactRect);
				sf::FloatRect ent2Rect = (interactComp2->_interactRect);

				if (ent1Rect.intersects(ent2Rect))
				{
					auto inputComp = it->second->getOwnerEntity()->get<InputComponent>();	//set input comp
					if (inputComp->InteractPressed)
					{
						luabridge::LuaRef HPTable = luabridge::getGlobal(GameManager::L, ot->second->getOwnerEntity()->getScriptTable().c_str());
						luabridge::LuaRef InteractTable = HPTable["InteractComponent"];
						luabridge::LuaRef interactFunc = InteractTable["interact"];
						//if (interactFunc.isFunction())
							//interactFunc(LuaEntityHandle(it->second->getOwnerEntity()), LuaEntityHandle(ot->second->getOwnerEntity()));
					}
				}
			}
		//}
	}*/
}

void InteractSystem::checkForInteractions(const EntityID& player) {
	//must be more than just player to interact with
	if (_entities.size() <= 1) { return; }

	EntityManager* entManager = _systemManager->getEntityManager();
	InteractComponent* interactComp = entManager->getComponent<InteractComponent>(player, ComponentType::Interact);
	PositionComponent* posComp = entManager->getComponent<PositionComponent>(player, ComponentType::Position);

	interactComp->_interactRect.left = posComp->_position.x - interactComp->_interactRect.width/2 + interactComp->_localInteractRectOffset.x;	//set bounding box posotion
	interactComp->_interactRect.top = posComp->_position.y - interactComp->_interactRect.height + interactComp->_localInteractRectOffset.y;

	for (auto& ent : _entities) {

		if (ent == player) { continue; }

		auto interactComp2 = entManager->getComponent<InteractComponent>(ent, ComponentType::Interact);
		auto posComp2 = entManager->getComponent<PositionComponent>(ent, ComponentType::Position);

		interactComp2->_interactRect.left = posComp2->_position.x - interactComp2->_interactRect.width/2 + interactComp2->_localInteractRectOffset.x;	//set bounding box posotion
		interactComp2->_interactRect.top = posComp2->_position.y - interactComp2->_interactRect.height + interactComp2->_localInteractRectOffset.y;

		sf::FloatRect ent1Rect = (interactComp->_interactRect);
		sf::FloatRect ent2Rect = (interactComp2->_interactRect);

		if (ent1Rect.intersects(ent2Rect))
		{
			sol::table HPTable = entManager->getLuaManager()->getLuaState()[entManager->getEntityInfo(ent).scriptTable.c_str()];	//setup lua link
			sol::table InteractTable = HPTable["InteractComponent"];
			sol::function interactFunc = InteractTable["interact"];
			if (interactFunc.valid()) {
				interactFunc(player, ent);
			}
		}
	}
}

void InteractSystem::handleEvent(const EntityID& ent, const EntityEvent& entEvent) {
	switch (entEvent) {
		case EntityEvent::Interacting:
			checkForInteractions(ent);
			break;
	}
}