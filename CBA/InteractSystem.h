#ifndef INTERACTSYS_H
#define INTERACTSYS_H

#include "System.h"

class InteractSystem : public System
{
public:
	InteractSystem(SystemManager*);
	void update(float) override;
	void handleEvent(const EntityID&, const EntityEvent&) override;

private:
	void checkForInteractions(const EntityID&);
};

#endif