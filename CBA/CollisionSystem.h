#ifndef COLLISIONSYS_H
#define COLLISIONSYS_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "System.h"
#include <fstream>

enum class Collision{ None, Solid };

class CollisionSystem : public System
{
public:
	CollisionSystem(SystemManager*);
	void update(float) override;
	void handleEvent(const EntityID&, const EntityEvent&) override;
	void loadCollisionMap(std::string&);

private:
	void checkForMapCollisions(const EntityID&, CollisionComponent*);
	void checkForSightOfPlayer(const EntityID&, const EntityID&, sol::table&);

	void moveObjectOutOfIntersection(sf::FloatRect, const EntityID&);
	//void moveObjectOutOfIntersection(PositionComponent*&, CollisionComponent*&, sf::FloatRect, sf::FloatRect, sf::FloatRect);
	sf::Vector2u getIntersectedTile(sf::Vector2f);
	unsigned int getLowerTestTile(unsigned int);

	unsigned int _width, _height;
	const sf::Vector2u _tileSize = sf::Vector2u(128, 128);
	std::vector<Collision> _levelCol;

	bool raycastForCollision(const sf::Vector2f, const sf::Vector2f);
};

#endif