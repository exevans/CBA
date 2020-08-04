#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "System.h"

class LogicSystem : public System
{
public:
	LogicSystem(SystemManager*);
	void update(float) override;	
	void handleEvent(const EntityID&, const EntityEvent&) override;
};

#endif
