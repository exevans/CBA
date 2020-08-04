#ifndef STATE_OPTIONS_MENU_H
#define STATE_OPTIONS_MENU_H

#include "State.h"

class EventDetails;

class State_OptionsMenu : public State {
public:
	State_OptionsMenu(StateManager* sManager) : State(sManager) {};
	void onCreate() override;
	void onDestroy() override;

	void activate() override;
	void deactivate() override;

	void update(float) override;
	void draw() override;

	void continueToMain(EventDetails*);

private:
};

#endif