#ifndef OBJECTFACT_H
#define OBJECTFACT_H

#include <vector>
#include <memory>
#include "System.h"
#include "Window.h"
#include <iostream>

class Entity;

class ObjectFactorySystem : public System
{
public:
	ObjectFactorySystem(SystemManager*);
	void update(float) override;
	void handleEvent(const EntityID&, const EntityEvent&) override;
	void loadLevel(std::string);
	void saveLevel();
	Entity* getPlayer();

private:
	void createEntity(const std::string&, const std::string&);
	void checkForNewEntitys();
	void checkForInactiveEntitys();
	void clearAllEntitys();

	Entity* getEntityByFileID(unsigned int);

	std::vector<std::shared_ptr<Entity>> _entitys;
	//Window* _window;
};

#endif
