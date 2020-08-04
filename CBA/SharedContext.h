#ifndef SHARED_CONTEXT_H
#define SHARED_CONTEXT_H

#include "TextureManager.h"

class EventManager;
class MapManager;
class SystemManager;
class EntityManager;
class TextManager;
class Window;
class LuaManager;
class NavMeshManager;
class LightManager;

struct SharedContext {
	SharedContext() : _window(nullptr), _eventManager(nullptr), _textureManager(nullptr), _systemManager(nullptr), _entityManager(nullptr) , _luaManager(nullptr), _navMeshManager(nullptr), _textManager(nullptr), _lightManager(nullptr) {};
	Window* _window;
	TextureManager* _textureManager;
	EventManager* _eventManager;
	SystemManager* _systemManager;
	EntityManager* _entityManager;
	LuaManager* _luaManager;
	NavMeshManager* _navMeshManager;
	MapManager* _mapManager;
	TextManager* _textManager;
	LightManager* _lightManager;
};

#endif