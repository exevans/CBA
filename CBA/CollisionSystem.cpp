#include <sstream>

#include "CollisionSystem.h"
#include "Entity.h"
#include "Component.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "KeyBinding.h"
#include "LuaManager.h"

CollisionSystem::CollisionSystem(SystemManager* sysManager)
	:System(sysManager, SystemType::Collision)
{
	addRequiredComponentType(ComponentType::Position);
	addRequiredComponentType(ComponentType::Collision);
}

void CollisionSystem::update(float deltaTime)
{
	EntityManager* entManager = _systemManager->getEntityManager();
	for (auto& ent : _entities) {
		CollisionComponent* colComp = entManager->getComponent<CollisionComponent>(ent, ComponentType::Collision);
		PositionComponent* posComp = entManager->getComponent<PositionComponent>(ent, ComponentType::Position);

		//component must be active
		if (!colComp->isActive()) { continue; }

		colComp->_colRect.left = (posComp->_position.x - colComp->_colRect.width/2 + colComp->_localColRectOffset.x);	//set bounding box position
		colComp->_colRect.top = (posComp->_position.y - colComp->_colRect.height + colComp->_localColRectOffset.y);

		//check out of bounds
		if (colComp->_colRect.left + colComp->_colRect.width > (this->_width * this->_tileSize.x) - 128)
			posComp->_position.x = (this->_width * this->_tileSize.x) - colComp->_localColRectOffset.x - colComp->_colRect.width/2 -128;
		if (colComp->_colRect.top + colComp->_colRect.height > (this->_height * this->_tileSize.y) - 128)
			posComp->_position.y = (this->_height * this->_tileSize.y) - colComp->_localColRectOffset.y - 128;

		if (entManager->getEntityInfo(ent).containsTag("Player") || entManager->getEntityInfo(ent).containsTag("Projectile"))	//PLAYER
		{
			//CHECK MAP COLLISIONS
			//sf::Vector2u entityTilePos = this->getIntersectedTile(sf::Vector2f(colComp->_colRect.left + colComp->_colRect.width / 2, colComp->_colRect.top + colComp->_colRect.height / 2));
			checkForMapCollisions(ent, colComp);
		}

		//check entity Collisions
		for (auto& ent2 : _entities) {
			CollisionComponent* colComp2 = entManager->getComponent<CollisionComponent>(ent2, ComponentType::Collision);
			PositionComponent* posComp2 = entManager->getComponent<PositionComponent>(ent2, ComponentType::Position);

			if ((ent != ent2) /*&& (!it->second->getOwnerEntity()->hasParent() || it->second->getOwnerEntity()->getParent()->getID() != ot->first)*/)
			{
				colComp2->_colRect.left = (posComp2->_position.x -colComp2->_colRect.width/2 + colComp2->_localColRectOffset.x)/* - colComp2->_colRect.width / 2*/;	//set bounding box position
				colComp2->_colRect.top = (posComp2->_position.y - colComp2->_colRect.height + colComp2->_localColRectOffset.y)/* - colComp2->_colRect.height / 2*/;

				sf::FloatRect area;
				sf::FloatRect ent1Rect = (colComp->_colRect);
				sf::FloatRect ent2Rect = (colComp2->_colRect);

				sol::table HPTable =  entManager->getLuaManager()->getEntityLuaRef(ent2);
				sol::table ColTable = HPTable["CollisionComponent"];

				if (ent1Rect.intersects(ent2Rect, area))
				{
					/*if (entManager->hasComponent(ent, ComponentType::Input))	//PLAYER
					{
						if (colComp2->_collisionType == Collision::Solid)	//SOLID
						{
							this->moveObjectOutOfIntersection(posComp, colComp, area, ent1Rect, ent2Rect);	//ditch when possible
						}
					}*/

					//all entities + Player
					//if (!ot->second->getOwnerEntity()->hasParent() || ot->second->getOwnerEntity()->getParent()->getID() != it->second->getOwnerEntity()->getID() && (!it->second->getOwnerEntity()->hasParent() || it->second->getOwnerEntity()->getParent()->getID() != ot->second->getOwnerEntity()->getParent()->getID()))
					{
						if (colComp2->collidedEntityID != ent)	//not already intersecting
						{
							colComp2->collidedEntityID = ent;
							sol::function collideEnter = ColTable["OnEnter"];
							if (collideEnter.valid())
								collideEnter(ent, ent2);
						}
						else if (colComp2->collidedEntityID == ent)	//already intersecting
						{
							colComp2->collidedEntityID = ent;
							sol::function collideStay = ColTable["OnStay"];
							if (collideStay.valid()) {
								collideStay(ent, ent2);
							}
						}
					}

				}	//not colliding
				else if (colComp2->collidedEntityID == ent)
				{
					colComp2->collidedEntityID = -1;
					sol::function collideExit = ColTable["OnExit"];
					if (collideExit.valid())
						collideExit(ent, ent2);
				}


				//do line of sight checks
				if (!entManager->getEntityInfo(ent).containsTag("Player") || !entManager->hasComponent(ent2, ComponentType::AI)) { continue; }
				checkForSightOfPlayer(ent, ent2, HPTable);
			}
		}
	}
}

void CollisionSystem::handleEvent(const EntityID& ent, const EntityEvent& entEvent) {

}

void CollisionSystem::loadCollisionMap(std::string& path)
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

	this->_width = mapRoot.attribute("width").as_int();
	this->_height = mapRoot.attribute("height").as_int();
	this->_levelCol.resize(this->_width * this->_height);

	for (unsigned int j = 0; j < _height; j++)
		for (unsigned int i = 0; i < _width; i++)
		{
			_levelCol[i + (j * _width)] = Collision::None;
		}

	pugi::xml_node tileSet = mapRoot.child("tileset");
	//_tileSets.push_back(tileSet.attribute("source").as_string());
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

	for (unsigned int j = 0; j < _height; j++) {
		for (unsigned int i = 0; i < _width; i++)
		{
			if (vect[i + (j * _width)] != 0) {
				_levelCol[i + (j * _width)] = Collision::Solid;
			}
		}
	}

	std::cout << "collision Map loaded" << std::endl;
}

void CollisionSystem::checkForMapCollisions(const EntityID& ent, CollisionComponent* colComp) {
	EntityManager* entManager = _systemManager->getEntityManager();

	//CHECK MAP COLLISIONS
	sf::Vector2u entityTilePos = this->getIntersectedTile(sf::Vector2f(colComp->_colRect.left + colComp->_colRect.width / 2, colComp->_colRect.top + colComp->_colRect.height / 2));

	for (unsigned int j = this->getLowerTestTile(entityTilePos.y); j < entityTilePos.y + 2; j++)
		for (unsigned int i = this->getLowerTestTile(entityTilePos.x); i < entityTilePos.x + 2; i++)
			if (_levelCol[i + (j * _width)] == Collision::Solid) {
				sf::FloatRect mapColTile;
				mapColTile.width = _tileSize.x;
				mapColTile.height = _tileSize.y;

				mapColTile.left = mapColTile.width * i;
				mapColTile.top = mapColTile.height * j;

				sf::FloatRect area;

				if (colComp->_colRect.intersects(mapColTile, area))
				{
					if (entManager->getEntityInfo(ent).containsTag("Player")) {
						moveObjectOutOfIntersection(mapColTile, ent);
					}
					else if (entManager->getEntityInfo(ent).containsTag("Projectile")) {
						LuaEntityHandle(entManager, ent).setInactive();
					}
				}
			}
}

void CollisionSystem::checkForSightOfPlayer(const EntityID& player, const EntityID& other, sol::table& Table) {
	EntityManager* entManager = _systemManager->getEntityManager();
	PositionComponent* posComp = entManager->getComponent<PositionComponent>(player, ComponentType::Position);
	PositionComponent* posComp2 = entManager->getComponent<PositionComponent>(other, ComponentType::Position);
	AIComponent* AIComp2 = entManager->getComponent<AIComponent>(other, ComponentType::AI);

	bool raycastToPlayer = raycastForCollision(posComp->_position, posComp2->_position);
	if (raycastToPlayer && !AIComp2->_viewOfPlayer) {
		//started viewing player
		AIComp2->_viewOfPlayer = true;

		sol::table AITable = Table["AIComponent"];
		sol::function viewEnter = AITable["OnViewEnter"];
		if (viewEnter.valid()) {
			viewEnter(player, other);
		}
	} else if (!raycastToPlayer && AIComp2->_viewOfPlayer) {
		//ended seeing player
		AIComp2->_viewOfPlayer = false;

		sol::table AITable = Table["AIComponent"];
		sol::function viewExit = AITable["OnViewExit"];
		if (viewExit.valid()) {
			viewExit(player, other);
		}
	}
}

void CollisionSystem::moveObjectOutOfIntersection(sf::FloatRect mapColTile, const EntityID& entity)
{
	int i = mapColTile.left / mapColTile.width;
	int j = mapColTile.top / mapColTile.height;
	
	EntityManager* entManager = _systemManager->getEntityManager();
	auto posComp = entManager->getComponent<PositionComponent>(entity, ComponentType::Position);
	auto colComp = entManager->getComponent<CollisionComponent>(entity, ComponentType::Collision);

	sf::FloatRect area;

	if (mapColTile.intersects(colComp->_colRect, area))
		if (area.height > area.width)	//left/right
		{
			if (area.contains(mapColTile.left, area.top))//left
			{
				if (_levelCol[i - 1 + (j * _width)] == Collision::None)
					posComp->_position.x = mapColTile.left - colComp->_localColRectOffset.x - colComp->_colRect.width / 2;
			}
			else if (_levelCol[i + 1 + (j * _width)] == Collision::None)//right
				posComp->_position.x = mapColTile.left - colComp->_localColRectOffset.x + mapColTile.width + colComp->_colRect.width / 2;
			_systemManager->addEvent(entity, (EventID)EntityEvent::Colliding_X);
		}
		else//Top/bottom
		{
			if (area.contains(area.left, mapColTile.top))//top
			{
				if (_levelCol[i + ((j - 1) * _width)] == Collision::None)
					posComp->_position.y = mapColTile.top - colComp->_localColRectOffset.y /*- colComp->_colRect.height*/;
			}
			else if (_levelCol[i + ((j + 1) * _width)] == Collision::None) //bottom
				posComp->_position.y = mapColTile.top - colComp->_localColRectOffset.y + mapColTile.height + colComp->_colRect.height;
			_systemManager->addEvent(entity, (EventID)EntityEvent::Colliding_Y);
		}
}

/*void CollisionSystem::moveObjectOutOfIntersection(PositionComponent*& posComp, CollisionComponent*& colComp, sf::FloatRect area, sf::FloatRect ent1Rect, sf::FloatRect ent2Rect)
{
	sf::Vector2f newPos = (posComp->_position);

	if (area.height > area.width)
	{
		if (area.contains(ent2Rect.left, area.top))
			newPos.x = ent2Rect.left - colComp->_localColRectOffset.x - ent1Rect.width / 2;
		else
			newPos.x = ent2Rect.left - colComp->_localColRectOffset.x + ent2Rect.width + ent1Rect.width / 2;
	}
	else
	{
		if (area.contains(area.left, ent2Rect.top))
			newPos.y = ent2Rect.top - colComp->_localColRectOffset.y - ent1Rect.height / 2;
		else
			newPos.y = ent2Rect.top - colComp->_localColRectOffset.y + ent2Rect.height + ent1Rect.height / 2;
	}
	//Set new Position
	posComp->_position = newPos;

	//Set not moving
	//if (posComp->getOwnerEntity()->hasComponent(std::type_index(typeid(MovementComponent))))
		//posComp->getOwnerEntity()->get<MovementComponent>()->_moving = false;
}*/

sf::Vector2u CollisionSystem::getIntersectedTile(sf::Vector2f entityPos)
{
	sf::Vector2u intersectedTilePos;

	intersectedTilePos.x = floor((entityPos.x) / 128);
	intersectedTilePos.y = floor((entityPos.y) / 128);

	return intersectedTilePos;
}

unsigned int CollisionSystem::getLowerTestTile(unsigned int mainTile)
{
	int lowestTile = mainTile - 1;

	if (lowestTile < 0)
		lowestTile = 0;

	return lowestTile;
}

bool CollisionSystem::raycastForCollision(const sf::Vector2f src, const sf::Vector2f dest) {
	//make line equation (y = mx + c)
	float eqM = ((src.y - dest.y) / (src.x - dest.x));
	float eqC = (src.y - (eqM * src.x));
	float testY = (eqM * src.x) + eqC;
	sf::Vector2i startSquare(src.x / 128, src.y / 128);
	sf::Vector2i currentSquare(startSquare);
	sf::Vector2i destinationSquare(dest.x / 128, dest.y / 128);

	if (destinationSquare.y > startSquare.y)
		currentSquare.y++;
	if (destinationSquare.x > startSquare.x)
		currentSquare.x++;

	while (currentSquare.x != destinationSquare.x) {
		//get relevant square
		float yInterceptFloatLocal = (eqM * currentSquare.x * 128) + eqC;
		int yIntercept = (eqM * currentSquare.x /** 128*/) + (eqC / 128);
		//int yInterceptLocal = yIntercept / 128;

		if (yIntercept >= 0 && yIntercept < _height) {
			//if intersects with solid then it has collided and the destination isnt visible
			if (src.x > dest.x) {
				if (currentSquare.x == 0) { return false; }

				/*if (int(yInterceptFloatLocal) % 128 == 0) {
				if (_tiles[currentSquare.x -1 + ((yIntercept)* _cellArraySize.x)] == true) {
				return false;
				}
				}*/

				if (_levelCol[currentSquare.x - 1 + ((yIntercept)* _width)] == Collision::Solid) {
					return false;
				}
			}
			else {
				if (_levelCol[currentSquare.x + ((yIntercept)* _width)] == Collision::Solid) {
					return false;
				}
			}
		}

		if (currentSquare.x > destinationSquare.x) {
			currentSquare.x--;
		}
		else if (currentSquare.x < destinationSquare.x) {
			currentSquare.x++;
		}
		else {
			break;
		}
	}

	while (currentSquare.y != destinationSquare.y) {
		//get relevant square
		float xInterceptFloatLocal = ((currentSquare.y * 128) - eqC) / (eqM);
		int xIntercept = (currentSquare.y - eqC / 128) / (eqM);

		if (xIntercept >= 0 && xIntercept < _width) {
			//if intersects with solid then it has collided and the destination isnt visible
			if (src.y > dest.y) {
				//if (currentSquare.y == 0) { return false; }
				if (int(xInterceptFloatLocal) % 128 == 0) {
					if (_levelCol[xIntercept - 1 + ((currentSquare.y - 1) * _width)] == Collision::Solid) {
						return false;
					}
				}


				if (_levelCol[xIntercept + ((currentSquare.y - 1) * _width)] == Collision::Solid) {
					return false;
				}
			}
			else {
				if (int(xInterceptFloatLocal) % 128 == 0) {
					if (_levelCol[xIntercept - 1 + ((currentSquare.y) * _width)] == Collision::Solid) {
						return false;
					}
				}

				if (_levelCol[xIntercept + (currentSquare.y * _width)] == Collision::Solid) {
					return false;
				}
			}
		}

		if (currentSquare.y > destinationSquare.y) {
			currentSquare.y--;
		}
		else if (currentSquare.y < destinationSquare.y) {
			currentSquare.y++;
		}
		else {
			break;
		}
	}
	return true;
}