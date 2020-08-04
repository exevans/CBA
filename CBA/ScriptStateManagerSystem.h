#ifndef SCRIPTSTATEMANAGE_H
#define SCRIPTSTATEMANAGE_H

#include "System.h"

class ScriptStateManagerSystem : public System
{
public:
	ScriptStateManagerSystem(SystemManager*);
	void update(float) override;
	void handleEvent(const EntityID&, const EntityEvent&) override;

private:
};

#endif