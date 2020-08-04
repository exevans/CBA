#ifndef TIMER_SYSTEM_H
#define TIMER_SYSTEM_H

#include "System.h"

class TimerSystem : public System {
public:
	TimerSystem(SystemManager*);
	void update(float) override;
	void handleEvent(const EntityID&, const EntityEvent&) override;
private:
};

#endif