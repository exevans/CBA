#include "NavMeshManager.h"
#include "EntityManager.h"

#include <pugixml.hpp>
#include <iostream>
#include <sstream>

Tile::Tile()
{

}

void Tile::setParent(int i, int j)
{
	_parentSquare.x = i;
	_parentSquare.y = j;
}

void Tile::reset()
{
	_GCost = 0;
	_HCost = 0;
	_FCost = 500;

	_openList = false;
	_closedList = false;
	_starterSquare = false;
	_finishSquare = false;
	_finalPath = false;
	_gCalculated = false;
	_testingTile = false;

	_nextSquare.x = 0;
	_nextSquare.y = 0;
}

void Tile::setH(int hDist)
{
	_HCost = hDist;
}

void Tile::setG(int distMoved)
{
	_GCost += distMoved;
}

int Tile::getG()
{
	return _GCost;
}

int Tile::getH()
{
	return _HCost;
}

int Tile::getF()
{
	_FCost = _HCost + _GCost;

	return _FCost;
}

sf::Vector2i Tile::getParent()
{
	return _parentSquare;
}

bool Tile::isWalkable()
{
	return _walkable;
}

void Tile::setUnwalkable()
{
	_walkable = false;
	//_shape.setOutlineColor(sf::Color::Red);
}

void Tile::setWalkable()
{
	_walkable = true;
	//_shape.setOutlineColor(sf::Color::White);
}



NavMeshManager::NavMeshManager()
{
	_tileShape.setSize(sf::Vector2f(128, 128));
	_tileShape.setOutlineThickness(2);
	_tileShape.setOutlineColor(sf::Color::White);
}

void NavMeshManager::setEntityManager(EntityManager* entManager)
{
	_entityManager = entManager;
}

void NavMeshManager::loadMap(std::string path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());

	if (!result)
	{
		std::cout << "XML [" << path << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
	}
	else
	{
		std::cout << "loaded" << std::endl;
	}

	pugi::xml_node mapRoot = doc.child("map");

	this->_cellArraySize.x = mapRoot.attribute("width").as_int();
	this->_cellArraySize.y = mapRoot.attribute("height").as_int();
	this->_tile.clear();
	this->_tile.resize(this->_cellArraySize.x * this->_cellArraySize.y);



	pugi::xml_node tileSet = mapRoot.child("tileset");
	pugi::xml_document tilesetDoc;
	std::string tileSource = "Assets/Maps/";
	tileSource += tileSet.attribute("source").as_string();
	tilesetDoc.load_file(tileSource.c_str());

	pugi::xml_node collisionLayer = mapRoot.find_child_by_attribute("layer", "name", "Collision");
	pugi::xml_node mapData = collisionLayer.child("data");

	std::string data = mapData.text().as_string();
	std::stringstream ss(data);
	std::vector<int> vect;
	int i;
	while (ss >> i)
	{
		vect.push_back(i);

		if (ss.peek() == ',')
			ss.ignore();
	}

	for (unsigned int j = 0; j < _cellArraySize.y; j++) {
		for (unsigned int i = 0; i < _cellArraySize.x; i++)
		{
			if (vect[i + (j * _cellArraySize.x)] != 0) {
				_tile[i + (j * _cellArraySize.x)].setUnwalkable();
			}
		}
	}
}

void NavMeshManager::getRouteToTarget(const EntityID& ent) {
	AIComponent* aiComp = _entityManager->getComponent<AIComponent>(ent, ComponentType::AI);
	PositionComponent* posComp = _entityManager->getComponent<PositionComponent>(ent, ComponentType::Position);
	MovementComponent* movComp = _entityManager->getComponent<MovementComponent>(ent, ComponentType::Movement);
	CollisionComponent* colComp = _entityManager->getComponent<CollisionComponent>(ent, ComponentType::Collision);

	calculateRoute(aiComp->_targetPosition, sf::Vector2f(colComp->_colRect.left + colComp->_colRect.width / 2, colComp->_colRect.top + colComp->_colRect.height / 2));

	if (this->_routeFinished)
	{
		aiComp->_routeNodes.clear();

		sf::Vector2i currentSquare = this->_startSquare;;

		Tile tile = _tile[currentSquare.x + (currentSquare.y * _cellArraySize.x)];
		while (!_tile[currentSquare.x + (currentSquare.y * _cellArraySize.x)]._finishSquare)
		{
			currentSquare = _tile[currentSquare.x + (currentSquare.y * _cellArraySize.x)]._nextSquare;
			aiComp->_routeNodes.push_back(sf::Vector2f(currentSquare * _blockSize) + sf::Vector2f(64.f, 64.f));
		}

		aiComp->_currentRouteNode = 0;
	}
}
/*void NavMeshManager::update(float deltaTime)
{
	int i = 0;
	EntityManager* entManager = _systemManager->getEntityManager();
	for (auto& ent : _entities) {
		AIComponent* aiComp = entManager->getComponent<AIComponent>(ent, ComponentType::AI);
		PositionComponent* posComp = entManager->getComponent<PositionComponent>(ent, ComponentType::Position);
		MovementComponent* movComp = entManager->getComponent<MovementComponent>(ent, ComponentType::Movement);
		CollisionComponent* colComp = entManager->getComponent<CollisionComponent>(ent, ComponentType::Collision);

		if (aiComp->calculatingRoot) {
			aiComp->calculatingRoot = false;
			calculateRoute(aiComp->_targetPosition, sf::Vector2f(colComp->_colRect.left + colComp->_colRect.width / 2, colComp->_colRect.top + colComp->_colRect.height / 2));

			if (this->_routeFinished)
			{
				aiComp->_routeNodes.clear();

				sf::Vector2i currentSquare = this->_startSquare;;

				Tile tile = _tile[currentSquare.x + (currentSquare.y * _cellArraySize.x)];
				while (!_tile[currentSquare.x + (currentSquare.y * _cellArraySize.x)]._finishSquare)
				{
					currentSquare = _tile[currentSquare.x + (currentSquare.y * _cellArraySize.x)]._nextSquare;
					aiComp->_routeNodes.push_back(sf::Vector2f(currentSquare * _blockSize) + sf::Vector2f(64.f, 64.f));
				}

				aiComp->_currentRouteNode = 0;
			}
		/*}
	}
}*/

void NavMeshManager::draw(Window* win)
{
	for (int i = 0; i < this->_cellArraySize.x; i++)
		for (int j = 0; j < this->_cellArraySize.y; j++) {
			Tile& cTile = _tile[i + (j * _cellArraySize.x)];

			_tileShape.setSize(sf::Vector2f(128, 128));
			_tileShape.setPosition(128 * i, 128 * j);

			if (cTile._finishSquare)
				_tileShape.setFillColor(sf::Color::Magenta);
			else if (cTile._starterSquare)
				_tileShape.setFillColor(sf::Color::Green);
			else if (cTile._testingTile)							//FOR TESTING TILES
				_tileShape.setFillColor(sf::Color::Yellow);
			else if (!cTile.isWalkable())
				_tileShape.setFillColor(sf::Color::Red);
			else
				_tileShape.setFillColor(sf::Color::Black);


			if (cTile._finalPath && !(cTile._finishSquare || cTile._starterSquare))
				_tileShape.setFillColor(sf::Color::Green);
			else if (cTile._closedList && !(cTile._finishSquare || cTile._starterSquare))
				_tileShape.setFillColor(sf::Color::Red);
			if ((cTile._finalPath || cTile._closedList) && !(cTile._finishSquare || cTile._starterSquare)) {
				_tileShape.setSize(sf::Vector2f(32, 32));
				_tileShape.setPosition((128 * i) + 46, (128 * j) + 46);
			}

			win->draw(_tileShape);
		}
}

void NavMeshManager::calculateRoute(sf::Vector2f targetPos, sf::Vector2f currentPos)
{
	bool valid = false;
	bool validStartSq = false;
	bool validFinishSq = false;
	int X = 0, Y = 0, K = 0, L = 0;

	this->_routeFinished = false;

	for (int i = 0; i < this->_cellArraySize.x; i++)
		for (int j = 0; j < this->_cellArraySize.y; j++)
		{
			//reset all squares
			_tile[i + (j * _cellArraySize.x)].reset();
		}

	this->_startSquare.x = currentPos.x / this->_blockSize;	//calculates the start square
	this->_startSquare.y = currentPos.y / this->_blockSize;
	if (_tile[this->_startSquare.x + (this->_startSquare.y * _cellArraySize.x)].isWalkable())
	{
		_tile[this->_startSquare.x + (this->_startSquare.y * _cellArraySize.x)]._starterSquare = true;
		validStartSq = true;
		X = this->_startSquare.x;
		Y = this->_startSquare.y;
	}

	this->_finishSquare.x = targetPos.x / this->_blockSize;	//calculates the finish square
	this->_finishSquare.y = targetPos.y / this->_blockSize;
	if (_tile[this->_finishSquare.x + (this->_finishSquare.y * _cellArraySize.x)].isWalkable())
	{
		_tile[this->_finishSquare.x + (this->_finishSquare.y * _cellArraySize.x)]._finishSquare = true;
		validFinishSq = true;
		K = this->_finishSquare.x;
		L = this->_finishSquare.y;
	}
	else if (_tile[this->_finishSquare.x + ((this->_finishSquare.y + 1) * _cellArraySize.x)].isWalkable())	//TEMP for when char is between 2 vertical walls with top being solid
	{
		this->_finishSquare.y++;
		_tile[this->_finishSquare.x + (this->_finishSquare.y * _cellArraySize.x)]._finishSquare = true;
		validFinishSq = true;
		K = this->_finishSquare.x;
		L = this->_finishSquare.y;
	}

	if (validFinishSq && validStartSq)
	{
		valid = true;
	}
	else {
		//for debug purposes
		valid = false;
	}

	int q = 0;
	while (valid && !_tile[K + (L * _cellArraySize.x)]._closedList && q < 200)		//repeat until endsquare is on closed list OR after 100 repeats
	{
		if (q == 0)
			repeatSearch(X, Y);	//passes starter square details
		else
		{
			X = findLowestF().x;
			Y = findLowestF().y;

			repeatSearch(X, Y);
		}
		q++;

	}

	if (valid && _tile[K + (L * _cellArraySize.x)]._closedList)	//if final square on closed list
	{
		findFinalRoute();
		_routeFinished = true;
	}
}

bool NavMeshManager::isDiagonalValid(int currentX, int nextX, int currentY, int nextY)
{
	//down
	if (nextY > currentY)
	{
		//right
		if (nextX > currentX)
		{
			if (_tile[((currentY + 1) * _cellArraySize.x) + currentX].isWalkable() && _tile[(currentY * _cellArraySize.x) + (currentX + 1)].isWalkable())
				return true;
		}
		//left
		else
		{
			if (_tile[((currentY + 1) * _cellArraySize.x) + currentX].isWalkable() && _tile[(currentY * _cellArraySize.x) + (currentX - 1)].isWalkable())
				return true;
		}
	}
	//up
	else
	{
		//right
		if (nextX > currentX)
		{
			if (_tile[((currentY - 1) * _cellArraySize.x) + currentX].isWalkable() && _tile[(currentY* _cellArraySize.x) + (currentX + 1)].isWalkable())
				return true;
		}
		//left
		else
		{
			if (_tile[((currentY - 1) * _cellArraySize.x) + currentX].isWalkable() && _tile[(currentY * _cellArraySize.x) + (currentX - 1)].isWalkable())
				return true;
		}
	}

	return false;
}

void NavMeshManager::repeatSearch(int i, int j)
{
	//inputs current sqaure
	_tile[i + (j * _cellArraySize.x)]._openList = true;

	for (int x = (i - 1); x <= (i + 1); x++)
		for (int y = j - 1; y <= (j + 1); y++)
			if (x >= 0 && x < _cellArraySize.x && y >= 0 && y < _cellArraySize.y)
				if ((i == x) || (j == y) || (isDiagonalValid(i, x, j, y)))				//ONLY ALLOWS SIDE MOEVENT NO DIAG
				{
					if (_tile[x + (y * _cellArraySize.x)].isWalkable())	//if actor knows scene
					{
						//checks for better path
						if (_tile[x + (y * _cellArraySize.x)]._openList)
							if (checkBetterPath(x, y, i, j))
							{
								_tile[x + (y * _cellArraySize.x)].setParent(i, j);
								_tile[x + (y * _cellArraySize.x)]._gCalculated = false;
							}

						if (!_tile[x + (y * _cellArraySize.x)]._openList && !_tile[x + (y * _cellArraySize.x)]._closedList)
							_tile[x + (y * _cellArraySize.x)].setParent(i, j);

						if (!_tile[x + (y * _cellArraySize.x)]._gCalculated)
						{
							if (i != x && j != y)				//sets gvalue for diagonal squares adds parent gcost
								_tile[x + (y * _cellArraySize.x)].setG(_gCostDiag + (_tile[(_tile[x + (y * _cellArraySize.x)].getParent().x) * (_tile[x + (y * _cellArraySize.x)].getParent().y)].getG()));	//needs fixing
							else
								//sets gvalue for horizontal squares
								_tile[x + (y * _cellArraySize.x)].setG(_gCostHor + (_tile[(_tile[x + (y * _cellArraySize.x)].getParent().x) * (_tile[x + (y * _cellArraySize.x)].getParent().y)].getG()));	//needs fixing

							_tile[x + (y * _cellArraySize.x)]._gCalculated = true;
						}

						int hDistToMoveX = this->_finishSquare.x - x;
						if (hDistToMoveX < 0)
							hDistToMoveX *= -1;

						int hDistToMoveY = this->_finishSquare.y - y;
						if (hDistToMoveY < 0)
							hDistToMoveY *= -1;

						int hTotal = hDistToMoveX + hDistToMoveY;

						_tile[x + (y * _cellArraySize.x)].setH(hTotal * _gCostHor);

						if (_tile[this->_finishSquare.x + (this->_finishSquare.y * _cellArraySize.x)]._openList)
							_tile[this->_finishSquare.x + (this->_finishSquare.y * _cellArraySize.x)]._closedList = true;

						if (!_tile[x + (y * _cellArraySize.x)]._closedList)
							_tile[x + (y * _cellArraySize.x)]._openList = true;
					}
				}
	_tile[i + (j * _cellArraySize.x)]._openList = false;
	_tile[i + (j * _cellArraySize.x)]._closedList = true;
}

void NavMeshManager::findFinalRoute()
{
	sf::Vector2i current;
	sf::Vector2i next;
	sf::Vector2i t;

	current = this->_finishSquare;
	next = this->_finishSquare;

	while (!_tile[this->_startSquare.x + (this->_startSquare.y * _cellArraySize.x)]._finalPath)
	{
		_tile[current.x + (current.y * _cellArraySize.x)]._finalPath = true;

		if (current.x != this->_startSquare.x || current.y != this->_startSquare.y)
		{
			t = _tile[current.x + (current.y * _cellArraySize.x)].getParent();

			//calcs next square for enemy to move to
			_tile[t.x + (t.y * _cellArraySize.x)]._nextSquare = current;

			current = t;
		}
	}

}

bool NavMeshManager::checkBetterPath(int MainX, int MainY, int posParentX, int posParentY)
{
	int oldG = _tile[MainX + (MainY * _cellArraySize.x)].getG();

	int newG = 0;

	if (posParentX != MainX && posParentY != MainY)
		newG = _gCostDiag + _tile[posParentX + (posParentY * _cellArraySize.x)].getG();
	else
		newG = _gCostHor + _tile[posParentX + (posParentY * _cellArraySize.x)].getG();

	if (newG < oldG)
		return true;
	else
		return false;
}

sf::Vector2i NavMeshManager::findLowestF()
{
	sf::Vector2i currentLowestFSquare;
	int currentLowest = 50000;

	for (int i = 0; i < _cellArraySize.x; i++)
		for (int j = 0; j < _cellArraySize.y; j++)
			if (_tile[i + (j * _cellArraySize.x)]._openList)
				if (_tile[i + (j * _cellArraySize.x)].getF() < currentLowest && _tile[i + (j * _cellArraySize.x)].getF() > 0)
				{
					currentLowest = _tile[i + (j * _cellArraySize.x)].getF();
					currentLowestFSquare.x = i;
					currentLowestFSquare.y = j;
				}

	return currentLowestFSquare;
}