#include "TimerSystem.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "LuaManager.h"

TimerSystem::TimerSystem(SystemManager* sysManager)
	:System(sysManager, SystemType::Timer)
{
	addRequiredComponentType(ComponentType::Timer);
}

void TimerSystem::update(float deltaTime)
{
	EntityManager* entManager = _systemManager->getEntityManager();
	for (auto& ent : _entities) {
		TimerComponent* timerComp = entManager->getComponent<TimerComponent>(ent, ComponentType::Timer);

		for (auto& timerData : timerComp->_timers) {
			if (!timerData.second._active) { continue; }

			timerData.second._elapsedTime += deltaTime;
			//check if current time has exceeded past time
			if (timerData.second._elapsedTime > timerData.second._tickTime) {
				//reset elapsed time
				timerData.second._elapsedTime = 0;

				if (!timerData.second._repeat)
					timerData.second._active = false;
				//call lua 
				sol::table HPTable = entManager->getLuaManager()->getEntityLuaRef(ent);	//setup lua link
				sol::table TimersTable = HPTable["TimerComponent"];
				sol::table TimerTable = TimersTable[timerData.second._tableIndex];
				sol::function tickFunc = TimerTable["OnTick"];
				if (tickFunc.valid()) {
					tickFunc(ent);
				}
			}
		}
	}
}

void TimerSystem::handleEvent(const EntityID& ent, const EntityEvent& entEvent) {

}
