#ifndef NAVMESHSYS_H
#define NAVMESHSYS_H

#include "System.h"
#include <fstream>

/*class Tile
{
public:
	Tile();
	//sf::RectangleShape & getshape();
	void setPosition(int, int);
	void setParent(int, int);
	//void update();
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
	//void draw(sf::RenderWindow &);

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
	//sf::RectangleShape _shape;
	//sf::FloatRect _shape;
};

class NavMeshSystem : public System
{
public:
	NavMeshSystem(SystemManager*);
	void loadMap(std::string);
	void update(float) override;
	void handleEvent(const EntityID&, const EntityEvent&) override;
	//void draw(sf::RenderWindow&);

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
};*/

#endif