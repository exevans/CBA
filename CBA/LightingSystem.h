#ifndef LIGHTING_SYSTEM_H
#define LIGHTING_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "System.h"
#include "SystemManager.h"

class LightingSystem : public System
{
public:
	LightingSystem(SystemManager*);
	void update(float) override;
	void handleEvent(const EntityID&, const EntityEvent&) override;
	void onNotify(const Message&) override;
private:

};


#endif