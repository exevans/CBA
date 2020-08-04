#ifndef SCHEDULE_SYSTEM_H
#define SCHEDULE_SYSTEM_H

#include "System.h"
#include "SystemManager.h"

class ScheduleSystem : public System {
public:
	ScheduleSystem(SystemManager*);
	void update(float) override;
	void handleEvent(const EntityID&, const EntityEvent&) override;
	void onNotify(const Message&) override;
private:
};

#endif