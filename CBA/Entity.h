#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <vector>
#include <bitset>
#include <map>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include "Component.h"
#include "GameManager.h"
#include "LuaEntityHandle.h"
#include <string>
#include "Subject.h"
#include "Observer.h"
#include "Effect.h"

extern "C"
{
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

//#include <LuaBridge.h>

namespace Entitys
{
	enum State{Running, Destroyed};
}

class Component;

class Entity : public Subject, public Observer, public std::enable_shared_from_this<Entity>
{
public:
	Entity();
	~Entity();
	bool hasTag(std::string);
	//void loadScript(luabridge::lua_State* L, const std::string&, const std::string&);
	void addComponent(std::shared_ptr<Component>);
	void removeComponents();
	bool hasComponent(std::type_index) const;
	void setParent(std::shared_ptr<Entity>);
	bool hasParent() const;
	Entity* getParent() const;
	unsigned int getComponentNum() const;
	const unsigned int getID() const;
	const std::string& getTypeID();
	const std::string& getName();
	const std::string& getScriptFilePath();
	const std::string& getScriptTable();
	const _Uint32t getComponentKey() const;

	Entitys::State _entityState = Entitys::Running;

	template <typename T>
	T* get() const{
		auto& it = _components.find(std::type_index(typeid(T)));
		if (it != _components.end())
		{
			return dynamic_cast<T*>(it->second.get());
		}
		return nullptr;
	}

	const bool isActive() const;
	void setInactive();

	void setFileID(unsigned int);
	const unsigned int getFileID() const;

	virtual void onNotify(Entity*, std::string) override;
private:

	std::vector<std::string> _tags;

	unsigned int _ID = 0;
	unsigned int _fileID = 0;
	std::string _entityTypeID;
	_Uint32t _componentsKey;
	bool _child = false;
	std::shared_ptr<Entity> _parent;
	std::string _name;

	std::string _scriptPath;
	std::string _scriptTable;

	static unsigned int _nextID;

	std::map<std::type_index, std::shared_ptr<Component>> _components;
};

#endif