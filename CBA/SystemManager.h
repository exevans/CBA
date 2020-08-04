#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "Window.h"
#include "System.h"
#include "SystemType.h"
#include "EventQueue.h"
#include "MessageHandler.h"
#include "SharedContext.h"

using EntityID = unsigned int;
class EntityManager;

using SystemContainer = std::unordered_map<SystemType, System*>;
using EntityEventContainer = std::unordered_map<EntityID, EventQueue>;

class SystemManager {
public:
	SystemManager(SharedContext*);
	~SystemManager();

	void setEntityManager(EntityManager*);
	EntityManager* getEntityManager();
	MessageHandler* getMessageHandler();

	template <class T>
	T* getSystem(const SystemType& sysType) {
		return dynamic_cast<T*>(_systems[sysType]);
	}

	void loadLevel(const std::string&);
	void saveLevel();
	void activatePlayer(); //temp

	void addEvent(const EntityID&, const EventID&);

	void entityModified(const EntityID&, const _Uint32t&);
	void update(float);
	void draw(Window*);

	void removeEntity(const EntityID&);

	void purgeSystems();
	void purgeEntities();

	SharedContext* getShared();
private:
	void handleEvents();

	SystemContainer _systems;
	EntityManager* _entityManager;
	LightManager* _lightingManager;
	EntityEventContainer _events;
	MessageHandler _messageHandler;
	SharedContext* _sharedContext;
};

#endif