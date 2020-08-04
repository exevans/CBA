#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "System.h"

enum class Axis {X, Y};

class MovementSystem : public System
{
public:
	MovementSystem(SystemManager*);
	void update(float) override;
	void movementStep(float, MovementComponent*, PositionComponent*);
	void handleEvent(const EntityID&, const EntityEvent&) override;
	void notify(const Message&);

private:
	void stopEntity(const EntityID&, const Axis&);
	void setDirection(const EntityID&, const Direction&);
};


#endif