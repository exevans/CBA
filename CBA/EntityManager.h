#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <string>

//#include <LuaBridge.h>

extern "C"
{
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#include "NavMeshManager.h"
#include "LuaEntityHandle.h"
#include "EntityEvent.h"
#include "Component.h"
#include "ComponentType.h"

using TagContainer = std::vector<std::string>;

struct EntityInfo {
	_Uint32t bitset = 0;
	std::string name = "";
	std::string scriptPath = "";
	std::string scriptTable = "";
	bool active = true;
	unsigned int fileID = 0;
	//should be somwhere else
	std::string destinationCellId = "";
	int destinationEntId = 0;

	bool containsTag(const std::string& tagName) {
		for (auto& tag : tags) {
			if (tag == tagName) {
				return true;
			}
		}
		return false;
	}

	TagContainer tags;
};

using EntityID = unsigned int;
using LuaHandleContainer = std::vector<EntityID>;//std::unordered_map<EntityID, LuaEntityHandle*>;
using ComponentContainer = std::vector<Component*>;
using EntityData = std::pair<EntityInfo, ComponentContainer>;
using EntityContainer = std::unordered_map<EntityID, EntityData>;
using ComponentFactory = std::unordered_map<ComponentType, std::function<Component*(void)>>;

class NavMeshManager;
class SystemManager;
class LuaManager;

class EntityManager{
public:
	EntityManager() {};
	EntityManager(SystemManager*, LuaManager*, NavMeshManager*);
	~EntityManager();

	void requestReloadLevel();
	void requestLoadLevel(const EntityID&);
	void requestLoadLevel(const std::string&);

	bool loadingLevel(std::string&);

	void loadLevel(const std::string&);
	void saveLevel();

	LuaManager* getLuaManager();
	NavMeshManager* getNavMeshManager();

	int addEntity();
	int addEntity(const std::string&, const std::string&);
	bool removeEntity(const EntityID&);
	bool addComponent(const EntityID&, const ComponentType&);
	void purge();
	bool hasEntity(const EntityID&);

	EntityInfo& getEntityInfo(const EntityID& entID) {
		return _entities.find(entID)->second.first;
	};

	const EntityID& getEntityIdFromFileId(unsigned int);

	template <class T>
	T* getComponent(const EntityID& ent, const ComponentType& comType) {
		auto itr = _entities.find(ent);
		if (itr == _entities.end()) {
			return nullptr;
		}
		auto& compCon = itr->second.second;
		auto comp = std::find_if(compCon.begin(), compCon.end(), [&comType](Component* c) {
			return c->getComponentType() == comType;
		});
		if (comp == compCon.end()) {
			return nullptr;
		}
		return dynamic_cast<T*>(*comp);
	}

	bool hasComponent(const EntityID&, const ComponentType&);

private:
	void loadLevelEntitys(const pugi::xml_node&);
	void loadEntityCallbacks(const pugi::xml_node&);

	template <class T>
	void addComponentType(const ComponentType& comp) {
		_componentFactory[comp] = []()->Component* {
			return new T();
		};
	}

	unsigned int _entityCounter;

	LuaHandleContainer _luaEntityHandle;

	EntityContainer _entities;
	ComponentFactory _componentFactory;

	SystemManager* _systemManager;
	LuaManager* _luaManager;
	NavMeshManager* _navMeshManager;

	bool _loadingNewLevel;
	std::string _levelToLoad;
	EntityID _destinationEntId;
};

#endif