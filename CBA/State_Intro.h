#ifndef STATE_INTRO_H
#define STATE_INTRO_H

#include <SFML/Graphics.hpp>
#include "State.h"

class EventDetails;

class State_Intro : public State {
public:
	State_Intro(StateManager* sManager) : State(sManager){};
	void onCreate() override;
	void onDestroy() override;

	void activate() override;
	void deactivate() override;

	void update(float) override;
	void draw() override;

	void continueToMain(EventDetails*);
private:
	sf::Texture _introTexture;
	sf::Sprite _introSprite;
	sf::Text _text;
	sf::RectangleShape _shape;
};

#endif