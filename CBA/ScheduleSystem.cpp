#include "ScheduleSystem.h"

#include "Component.h"
#include "EntityMessage.h"
#include "Entity.h"
#include <iostream>

ScheduleSystem::ScheduleSystem(SystemManager* sysManager)
	:System(sysManager, SystemType::Schedule)
{
	addRequiredComponentType(ComponentType::Schedule);
}

void ScheduleSystem::update(float deltaTime)
{
	EntityManager* entManager = _systemManager->getEntityManager();
	for (auto ent : _entities) {
		ScheduleComponent* schedComp = entManager->getComponent<ScheduleComponent>(ent, ComponentType::Schedule);
		if (schedComp->_tasks.size() <= schedComp->_currentTaskIndex) { continue; }

		std::string& currentTask = schedComp->_tasks[schedComp->_currentTaskIndex].first;
		ScheduleData& taskData = schedComp->_tasks[schedComp->_currentTaskIndex].second;

		//resets changing task if needed to tell when entering task for first time
		bool changingTask = schedComp->_changingTask;
		schedComp->_changingTask = false;

		if (currentTask == "GoTo") {
			if (changingTask) {
				//AIComponent* aiComp = entManager->getComponent<AIComponent>(ent, ComponentType::AI);
				LuaEntityHandle(entManager, ent).setRouteByNavMesh(taskData._vect2f);
			}

			LuaEntityHandle(entManager, ent).followRoute();
		}
		else if (currentTask == "Patrol_Route") {
			//set first point calculated to false first time
			if (changingTask) {
				entManager->getComponent<AIComponent>(ent, ComponentType::AI)->_firstPointCalculated = false;
			}
			LuaEntityHandle(entManager, ent).patrol();
			LuaEntityHandle(entManager, ent).followRoute();
		}
		else if (currentTask == "Repeat_Sch") {
			schedComp->resetSchedule();
		}
		else if (currentTask == "Tog_Con") {
			InputComponent* inComp = entManager->getComponent<InputComponent>(ent, ComponentType::Input);
			inComp->_controllable = !inComp->_controllable;
			schedComp->advanceCurrentTask();
		}
		else if (currentTask == "Lod_Lev") {
			_systemManager->getEntityManager()->requestLoadLevel(taskData._int);
			schedComp->advanceCurrentTask();
		}
		else if (currentTask == "Change_State") {
			LuaEntityHandle(entManager, ent).setScriptState(taskData._str);
			schedComp->advanceCurrentTask();
		}
	}
}

void ScheduleSystem::handleEvent(const EntityID& ent, const EntityEvent& entEvent) {

}

void ScheduleSystem::onNotify(const Message& message) {

}