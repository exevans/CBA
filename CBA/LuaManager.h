#ifndef LUA_MANAGER_H
#define LUA_MANAGER_H

//#include <LuaBridge.h>
#include <unordered_map>

extern "C"
{
	# include "lua.h"
	# include "lauxlib.h"
	# include "lualib.h"
}
#include <sol.hpp>

class EntityManager;

using EntityID = unsigned int;
using ScriptContainer = std::unordered_map<EntityID, sol::table>;

//for Lua Observers
using LuaEntityCallback = std::pair<EntityID, std::string>;
using LuaEntityCallbackContainer = std::vector<LuaEntityCallback>;
using LuaEventObservers = std::unordered_map<std::string, LuaEntityCallbackContainer>;
using LuaEntityObservers = std::unordered_map<EntityID, LuaEventObservers>;

class LuaManager {
public:
	LuaManager();
	~LuaManager();
	void setEntityManager(EntityManager*);
	sol::state& getLuaState();
	void loadEntityLua(const EntityID&, const std::string& scriptPath, const std::string& scriptTable);
	const sol::table& getEntityLuaRef(const EntityID&);
	void purgeScripts();

	bool subscribeToLuaEvent(const EntityID&, const EntityID&, const std::string&, const std::string&);
	bool unsubscribeFromLuaEvent(const EntityID&, const EntityID&, const std::string&);
	void notifyObservers(const EntityID&, const std::string&);
	void purgeObservers();
	void purge();
	void removeEntityFromObservers(const EntityID&);
	void removeEntity(const EntityID& ent);
private:
	void registerLua();
	bool addLuaEvent(const EntityID&, const std::string&);

	sol::state _L;
	EntityManager* _entityManager;
	ScriptContainer _entityScripts;
	LuaEntityObservers _luaEntityObservers;
};

#endif