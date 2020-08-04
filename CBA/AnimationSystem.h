#ifndef ANIMATIONSYS_H
#define ANIMATIONSYS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "System.h"
#include "SystemManager.h"

class AnimationSystem : public System
{
public:
	AnimationSystem(SystemManager*);
	void update(float) override;
	void handleEvent(const EntityID&, const EntityEvent&) override;
	void onNotify(const Message&) override;
private:

};

#endif