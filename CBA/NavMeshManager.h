#ifndef NAVMESH_MANAGER_H
#define NAVMESH_MANAGER_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Window.h"

class EntityManager;

using EntityID = unsigned int;

class Tile
{
public:
	Tile();
	void setParent(int, int);
	void reset();
	void setH(int);
	void setG(int);
	int getG();
	int getH();
	int getF();
	sf::Vector2i getParent();
	bool isWalkable();
	void setUnwalkable();
	void setWalkable();

	bool _starterSquare = false;
	bool _testingTile = false;
	bool _finishSquare = false;
	bool _openList = false;
	bool _closedList = false;
	bool _gCalculated = false;
	bool _finalPath = false;
	sf::Vector2i _nextSquare;
private:
	bool _walkable = true;
	//bool actorWalkable = true;	//makes tile spotting dynamic

	//bool seenByActor = false;
	float _friction;

	unsigned int _FCost = 500;
	int _HCost = 0;
	int _GCost = 0;

	sf::Vector2i _parentSquare;

	//sf::FloatRect _shape;
};

class NavMeshManager
{
public:
	NavMeshManager();
	void loadMap(std::string);
	void getRouteToTarget(const EntityID&);
	void setEntityManager(EntityManager*);
	void draw(Window*);

private:
	void calculateRoute(sf::Vector2f, sf::Vector2f);
	void repeatSearch(int, int);
	bool isDiagonalValid(int, int, int, int);
	void findFinalRoute();
	bool checkBetterPath(int, int, int, int);
	sf::Vector2i findLowestF();

	std::vector<Tile> _tile;
	bool _routeFinished = false;
	const int _blockSize = 128;
	const int _gCostDiag = 14;
	const int _gCostHor = 10;

	sf::Vector2i _startSquare;
	sf::Vector2i _finishSquare;

	sf::Vector2i _cellArraySize;

	EntityManager* _entityManager;
	sf::RectangleShape _tileShape;
};

#endif