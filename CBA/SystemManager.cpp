#include "SystemManager.h"
#include "ObjectFactory.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"
//#include "NavMeshSystem.h"
#include "MovementSystem.h"
#include "InteractSystem.h"
#include "CollisionSystem.h"
#include "DialogueManager.h"
#include "GameLogic.h"
#include "AnimationSystem.h"
#include "EntityManager.h"
#include "GameManager.h"
#include "ScheduleSystem.h"
#include "ScriptStateManagerSystem.h"
#include "TimerSystem.h"
#include "LightingSystem.h"

SystemManager::SystemManager(SharedContext* context) : _sharedContext(context), _entityManager(nullptr) {
	_systems[SystemType::Input] = new InputSystem(this);
	_systems[SystemType::Movement] = new MovementSystem(this);
	_systems[SystemType::Collision] = new CollisionSystem(this);
	_systems[SystemType::Interact] = new InteractSystem(this);
	_systems[SystemType::Timer] = new TimerSystem(this);
	_systems[SystemType::Logic] = new LogicSystem(this);
	_systems[SystemType::Animation] = new AnimationSystem(this);
	_systems[SystemType::Graphics] = new GraphicsSystem(this);
	_systems[SystemType::ScriptState] = new ScriptStateManagerSystem(this);
	_systems[SystemType::Schedule] = new ScheduleSystem(this);
	_systems[SystemType::Lighting] = new LightingSystem(this);
}

SystemManager::~SystemManager() {
	purgeSystems();
}

void SystemManager::setEntityManager(EntityManager* entityManager) {
	_entityManager = entityManager;
}

EntityManager* SystemManager::getEntityManager() {
	return _entityManager;
}

MessageHandler* SystemManager::getMessageHandler() {
	return &_messageHandler;
}

void SystemManager::purgeSystems() {
	for (auto& system : _systems) {
		delete system.second;
	}

	_systems.clear();
}

void SystemManager::purgeEntities() {
	for (auto& sys : _systems) {
		sys.second->purge();
	}
}

void SystemManager::removeEntity(const EntityID& entity) {
	for (auto system : _systems) {
		system.second->removeEntity(entity);
	}
}

void SystemManager::loadLevel(const std::string& path) {
	CollisionSystem* colSys = (CollisionSystem*)_systems[SystemType::Collision];
	colSys->loadCollisionMap("Assets/Maps/" + path + ".tmx");

	//NavMeshSystem* navSys = (NavMeshSystem*)_systems[SystemType::NavMesh];
	//navSys->loadMap("Assets/Maps/" + path + ".tmx");

	//getEntityManager()->getNavMeshManager()->loadMap("Assets/Maps/" + path + ".tmx");

	if (GameManager::_currentCell != "Null")
		this->saveLevel();
}

void SystemManager::saveLevel() {

}

void SystemManager::activatePlayer() {
	
}

void SystemManager::update(float deltaTime) {
	for (auto& system : _systems) {
		system.second->update(deltaTime);
	}

	handleEvents();
}

void SystemManager::addEvent(const EntityID& entityId, const EventID& eventId) {
	_events[entityId].addEvent(eventId);
}

void SystemManager::entityModified(const EntityID& entityId, const _Uint32t& newBitset) {
	for (auto& system : _systems) {
		System* sys = system.second;
		//check for adding new comps
		if (!sys->hasEntity(entityId)) {
			if ((sys->getSystemLockID() & newBitset) == sys->getSystemLockID()) {
				sys->addEntity(entityId);
			}
		}
		else {
			//remove if compcode no longer matches
			if ((sys->getSystemLockID() & newBitset) != sys->getSystemLockID()) {
				sys->removeEntity(entityId);
			}
		}
	}
}

void SystemManager::handleEvents() {
	for (auto& event : _events) {
		EventID id = 0;
		//sets id to the top event id to be processed
		while (event.second.processEvents(id)) {
			for (auto& system : _systems) {
				if (system.second->hasEntity(event.first)) {
					system.second->handleEvent(event.first, (EntityEvent)id);
				}
			}

			//special entityManager events are processed here
			if ((EntityEvent)id == EntityEvent::Despawned) {
				_entityManager->removeEntity(event.first);
				break;
			}
		}
	}
}

SharedContext* SystemManager::getShared() {
	return _sharedContext;
}

void SystemManager::draw(Window* window) {
	auto itr = _systems.find(SystemType::Graphics);
	if (itr == _systems.end()) {
		return;
	}
	GraphicsSystem* graphSys = (GraphicsSystem*)itr->second;
	graphSys->draw(window);

}