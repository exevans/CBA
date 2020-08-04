#ifndef STATE_PAUSED_H
#define STATE_PAUSED_H

#include "State.h"

class EventDetails;

class State_Paused : public State {
public:
	State_Paused(StateManager* sManager) : State(sManager){};
	void onCreate() override;
	void onDestroy() override;

	void activate() override;
	void deactivate() override;

	void update(float) override;
	void draw() override;

	void returnToGame(EventDetails*);
private:
	sf::Text _text;
	sf::Font _font;
	sf::RectangleShape _background;
};

#endif