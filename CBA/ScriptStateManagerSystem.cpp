#include "ScriptStateManagerSystem.h"
#include "Component.h"
#include "Entity.h"
#include "SystemManager.h"
#include "LuaManager.h"

//#include <LuaBridge.h>


ScriptStateManagerSystem::ScriptStateManagerSystem(SystemManager* sysManager)
	: System(sysManager, SystemType::ScriptState)
{
	addRequiredComponentType(ComponentType::ScriptState);
}

void ScriptStateManagerSystem::update(float deltaTime)
{
	EntityManager* entManager = _systemManager->getEntityManager();
	for (auto& ent : _entities) {
		ScriptStateMachineComponent* scriptComp = entManager->getComponent<ScriptStateMachineComponent>(ent, ComponentType::ScriptState);

		sol::table Table = entManager->getLuaManager()->getEntityLuaRef(ent);
		sol::table StateComp = Table["ScriptStateMachineComponent"];
		sol::table States = StateComp["States"];

		if (scriptComp->_changingState)	//changing states
		{
			scriptComp->_changingState = false;

			//exit function of old state
			sol::table StateTable = States[scriptComp->_currentState.c_str()];
			if (StateTable.valid())
			{
				sol::function exitFunc = StateTable["exit"];
				if (exitFunc.valid()) {
					exitFunc(ent);
				}
			}

			//enter function of new state
			StateTable = States[scriptComp->_newState.c_str()];
			if (StateTable.valid())
			{
				scriptComp->_currentState = scriptComp->_newState;

				sol::function enterFunc = StateTable["enter"];
				if (enterFunc.valid()) {
					enterFunc(ent);
				}
			}
		}

		//updates states every frame
		/*sol::table StateTable = States[scriptComp->_currentState.c_str()];
		if (StateTable.valid()) {
			sol::function executeFunc = StateTable["execute"];
			if (executeFunc.valid())
				executeFunc(ent);
		}*/
	}
}

void ScriptStateManagerSystem::handleEvent(const EntityID& ent, const EntityEvent& entEvent) {

}

