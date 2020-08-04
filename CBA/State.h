#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>
#include <iostream>

class StateManager;

class State {
public:
	State(StateManager* sManager) : _stateManager(sManager), _transparent(false), _trancendent(false) {}
	virtual ~State() = default;

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void activate() = 0;
	virtual void deactivate() = 0;

	virtual void update(float) = 0;
	virtual void draw() = 0;

	sf::View& getView() { return _view; };

	void setTransparent(bool input) { _transparent = input; };
	bool isTransparent() const { return _transparent; };
	void setTrancendent(bool input) { _trancendent = input; };
	bool isTrancendent() const { return _trancendent; };

protected:
	sf::View _view;
	bool _transparent;
	bool _trancendent;
	StateManager* _stateManager;
};

#endif