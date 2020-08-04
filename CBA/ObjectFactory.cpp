#include "ObjectFactory.h"
#include "Entity.h"
#include "Component.h"
#include <iostream>
#include <map>
#include "DialogueManager.h"
#include "pugixml.hpp"

ObjectFactorySystem::ObjectFactorySystem(SystemManager* sysManager) 
	: System(sysManager, SystemType::ScriptState)
{

}

void ObjectFactorySystem::update(float deltaTime)
{
	//check for new entitys to add
	this->checkForNewEntitys();
	//check for inactive entitys to remove
	this->checkForInactiveEntitys();
}

void ObjectFactorySystem::checkForNewEntitys()
{
	std::vector<std::shared_ptr<Entity>>::size_type size = _entitys.size();
	for (std::vector<std::shared_ptr<Entity>>::size_type i = 0; i < size; ++i)
	{
		if (_entitys[i]->hasComponent(std::type_index(typeid(InventoryComponent))))	//check active item in inventory of an entity
			if (_entitys[i]->get<InventoryComponent>()->_items.size() > 0)
			{
				if (_entitys[i]->get<InventoryComponent>()->getItem(_entitys[i]->get<InventoryComponent>()->_equipedItemID)->hasComponent(std::type_index(typeid(SpawnerComponent))))
				{
					auto item = _entitys[i]->get<InventoryComponent>()->getEquipedItem();
					if (item->get<SpawnerComponent>()->_spawning)
					{
						//creates entity to eject
						this->createEntity("Assets/Scripts/Items.lua", item->get<SpawnerComponent>()->_spawnTableName);

						std::shared_ptr<Entity> spawnItem = std::shared_ptr<Entity>(_entitys[_entitys.size() - 1]);
						spawnItem->setParent(_entitys[i]);
						spawnItem->addObserver(*_entitys[i].get());	//set parent as an obsever

						spawnItem->get<PositionComponent>()->_position = sf::Vector2f(_entitys[i]->get<PositionComponent>()->_position.x + _entitys[i]->get<TextureComponent>()->_sprite.getGlobalBounds().width / 2, _entitys[i]->get<PositionComponent>()->_position.y + _entitys[i]->get<TextureComponent>()->_sprite.getGlobalBounds().height / 2);//SET POSITION TO OWNER
						auto colComp = spawnItem->get<CollisionComponent>();
						colComp->_colRect.left = (spawnItem->get<PositionComponent>()->_position.x + colComp->_localColRectOffset.x);	//set bounding box position
						colComp->_colRect.top = (spawnItem->get<PositionComponent>()->_position.y + colComp->_localColRectOffset.y);

						if (_entitys[i]->hasTag("Player") && spawnItem->hasComponent(std::type_index(typeid(MovementComponent))))	//FOR PLAYER
						{
							//sf::Vector2i pixPos = _window.getMousePixPos();
							//sf::Vector2f worldPos = _window->getMouseWorldPos();//_window.mapPixelToCoords(pixPos);
							//LuaEntityHandle(spawnItem.get()).moveTowardsPosition(worldPos);	//set direction to mouse as target
						}
						/*else if (_entitys[i]->hasComponent(std::type_index(typeid(AIComponent))) && _entitys[i]->get<AIComponent>()->_enemy != nullptr && spawnItem->hasComponent(std::type_index(typeid(MovementComponent))))    //FOR NOT PLAYER
							LuaEntityHandle(spawnItem.get()).moveTowardsPosition(_entitys[i]->get<AIComponent>()->_enemy->get<PositionComponent>()->_position);	//set direction to mouse as target

						item->get<SpawnerComponent>()->_spawning = false;*/
					}
				}
			}
	}
}

void ObjectFactorySystem::handleEvent(const EntityID& ent, const EntityEvent& entEvent) {

}

void ObjectFactorySystem::createEntity(const std::string& scriptFilename, const std::string& tableName)
{
	std::shared_ptr<Entity> newEntity(new Entity());
	//newEntity->loadScript(GameManager::L, scriptFilename, tableName);
	this->notify(newEntity.get(), "EVENT_ENTITY_ADDED");
	_entitys.push_back(std::move(newEntity));

	//std::cout << "New char from XML " << newEntity->getName() << std::endl;
}

void ObjectFactorySystem::checkForInactiveEntitys()
{
	/*int i = 0;
	while (i < _entitys.size())
	{
		if (!_entitys[i]->isActive())
		{
			if (i == 3)
				int ytu = 4;
			//replace with observer call
			for (size_t j = 0; j < _entitys.size(); j++)
			{
				if (_entitys[j]->hasComponent(std::type_index(typeid(CollisionComponent))))	//stops inactive object collisions
				{
					auto colComp = _entitys[j]->get<CollisionComponent>();
					if (colComp->collidedEntityID == _entitys[i]->getID())	
					{
						luabridge::LuaRef HPTable = luabridge::getGlobal(GameManager::L, (_entitys)[j]->getScriptTable().c_str());
						luabridge::LuaRef ColTable = HPTable["CollisionComponent"];

						(_entitys)[j]->get<CollisionComponent>()->collidedEntityID = -1;
						luabridge::LuaRef collideExit = ColTable["OnExit"];
						/*if (collideExit.isFunction())
							collideExit(LuaEntityHandle(_entitys[i].get()), LuaEntityHandle(_entitys[j].get()));
					}
				}
			}

			if (_entitys[i]->hasComponent(std::type_index(typeid(InventoryComponent))))
			{
				auto inventComp = _entitys[i]->get<InventoryComponent>();

				if (inventComp->_items.size() > 0)
				{
					this->createEntity("Assets/Scripts/Items.lua", "Sack");
					_entitys[_entitys.size() - 1]->get<PositionComponent>()->_position = _entitys[i]->get<PositionComponent>()->_position;
					//add code to remove items
					for (auto it : inventComp->_items)
						if (!it->hasTag("NotDropable"))
						{
							_entitys[_entitys.size() - 1]->get<InventoryComponent>()->addEntity(it);
						}
				}
			}
			_entitys[i].get()->notify(_entitys[i].get(), "EVENT_ENTITY_REMOVED");	//notify entitys of entity removed
			this->notify(nullptr, "EVENT_ENTITY_REMOVED");	//notify other systems of entity removed
			_entitys.erase(_entitys.begin() + i);
		}
		else
			i++;
	}*/
}

void ObjectFactorySystem::clearAllEntitys()
{
	_entitys.clear();	//clears all of vector
}

void ObjectFactorySystem::loadLevel(std::string path)	//load from data file
{
	this->clearAllEntitys();
	
	//load level data
	pugi::xml_document doc;
	doc.load_file(path.c_str());

	pugi::xml_node mapRoot = doc.child("map");
	pugi::xml_node objects = mapRoot.child("objectgroup");


	//load current level data
	pugi::xml_document savedLevelDoc;
	savedLevelDoc.load_file("Assets/Data/Saves/TempObjectData.xml");

	//load current player data
	pugi::xml_node savedPlayerRoot = savedLevelDoc.child("Player").first_child();

	this->createEntity("Characters.lua", "player");

	if (!savedPlayerRoot.empty())
	{
		std::shared_ptr<Entity> newEntity = _entitys[_entitys.size() - 1];

		//pos should be saved per level
		if (GameManager::_triggerId != 0)
			newEntity->get<PositionComponent>()->_position = sf::Vector2f(objects.find_child_by_attribute("object", "id", std::to_string(GameManager::_triggerId).c_str()).attribute("x").as_float() + 15 - newEntity->get<TextureComponent>()->_textureSize.x / 2, objects.find_child_by_attribute("object", "id", std::to_string(GameManager::_triggerId).c_str()).attribute("y").as_float() - newEntity->get<TextureComponent>()->_textureSize.y / 2);	//sets the position
		else if (savedPlayerRoot.attribute("positionX"))
			newEntity->get<PositionComponent>()->_position = sf::Vector2f(savedPlayerRoot.attribute("positionX").as_float(), savedPlayerRoot.attribute("positionY").as_float());	//sets the position

		newEntity->get<InventoryComponent>()->clearInventory();	//clears the invent so it can be loaded feom file
		pugi::xml_node inventRoot = savedPlayerRoot.child("Inventory");
		for (pugi::xml_node item = inventRoot.first_child(); item; item = item.next_sibling())	//for all saved items in temp file
		{
			//Entity* newItemEntity = new Entity();
			std::shared_ptr<Entity> newItemEntity(new Entity);
			//newItemEntity->loadScript(GameManager::L, item.attribute("ScriptPath").as_string(), item.attribute("ScriptTable").as_string());

			newEntity->get<InventoryComponent>()->addEntity(std::move(newItemEntity));
		}
	}
	else
	{
		std::shared_ptr<Entity> player = _entitys[_entitys.size() - 1];

		player->get<PositionComponent>()->_position = sf::Vector2f(100.f, 100.f);
	}

	//load current level data
	pugi::xml_node savedLevelRoot = savedLevelDoc.child("Cells").find_child_by_attribute("Cell", "CellID", GameManager::_newCell.c_str());

	if (!savedLevelRoot.empty())	//if a temp file exists for the cell
	{
		for (pugi::xml_node savedItem = savedLevelRoot.first_child(); savedItem; savedItem = savedItem.next_sibling()) {
			for (pugi::xml_node object = objects.first_child(); object; object = object.next_sibling())
			{
				pugi::xml_node properties = object.child("properties");
				if (object.attribute("id").as_uint() == savedItem.attribute("fileID").as_uint())
				{
					this->createEntity(properties.find_child_by_attribute("property", "name", "scriptPath").attribute("value").as_string(), properties.find_child_by_attribute("property", "name", "scriptTable").attribute("value").as_string());
					std::shared_ptr<Entity> newEntity = _entitys[_entitys.size() - 1];

					if (object.attribute("id"))
						newEntity->setFileID(object.attribute("id").as_uint());	//sets the fileID
					if (object.attribute("x") && newEntity->hasComponent(std::type_index(typeid(TextureComponent))))
						newEntity->get<PositionComponent>()->_position = sf::Vector2f((object.attribute("x").as_float()), (object.attribute("y").as_float() - newEntity->get<TextureComponent>()->_textureSize.y));	//sets the position			
					else if (object.attribute("x") && newEntity->hasComponent(std::type_index(typeid(CollisionComponent))))
						newEntity->get<PositionComponent>()->_position = sf::Vector2f(object.attribute("x").as_float(), object.attribute("y").as_float());	//sets the position
					if (properties.find_child_by_attribute("property", "name", "destinationCellId"))
						newEntity->get<PositionComponent>()->_destinationCellId = properties.find_child_by_attribute("property", "name", "destinationCellId").attribute("value").as_string();
					if (properties.find_child_by_attribute("property", "name", "destinationTriggerId"))
						newEntity->get<PositionComponent>()->_destinationTriggerId = properties.find_child_by_attribute("property", "name", "destinationTriggerId").attribute("value").as_uint();

					break;
				}
			}
		}
	}
	else
	{
		for (pugi::xml_node object = objects.first_child(); object; object = object.next_sibling())
		{
			pugi::xml_node properties = object.child("properties");

			this->createEntity(properties.find_child_by_attribute("property", "name", "scriptPath").attribute("value").as_string(), properties.find_child_by_attribute("property", "name", "scriptTable").attribute("value").as_string());
			std::shared_ptr<Entity> newEntity = _entitys[_entitys.size() - 1];

			if (object.attribute("id"))
				newEntity->setFileID(object.attribute("id").as_uint());	//sets the fileID
			if (object.attribute("x") && newEntity->hasComponent(std::type_index(typeid(TextureComponent))))
				newEntity->get<PositionComponent>()->_position = sf::Vector2f((object.attribute("x").as_float()/* + newEntity->get<TextureComponent>()->_textureSize.x/2*/), (object.attribute("y").as_float() - newEntity->get<TextureComponent>()->_textureSize.y));	//sets the position			
			else if (object.attribute("x") && newEntity->hasComponent(std::type_index(typeid(CollisionComponent))))
				newEntity->get<PositionComponent>()->_position = sf::Vector2f(object.attribute("x").as_float() /*+ newEntity->get<CollisionComponent>()->_colRect.width / 2*/, object.attribute("y").as_float() /*+ newEntity->get<CollisionComponent>()->_colRect.height / 2*/);	//sets the position
			if (properties.find_child_by_attribute("property", "name", "destinationCellId"))
				newEntity->get<PositionComponent>()->_destinationCellId = properties.find_child_by_attribute("property", "name", "destinationCellId").attribute("value").as_string();
			if (properties.find_child_by_attribute("property", "name", "destinationTriggerId"))
				newEntity->get<PositionComponent>()->_destinationTriggerId = properties.find_child_by_attribute("property", "name", "destinationTriggerId").attribute("value").as_uint();
		}
	}

	for (pugi::xml_node object = objects.first_child(); object; object = object.next_sibling())
	{
		/*pugi::xml_node properties = object.child("properties").find_child_by_attribute("property", "name", "observeID");
		//for (pugi::xml_node observee = field.first_child(); observee; observee = observee.next_sibling())
		if (!properties.empty())
		{
			std::string data = properties.attribute("value").as_string();
			std::stringstream ss(data);
			std::vector<int> vect;
			int i;
			while (ss >> i)
			{
				vect.push_back(i);

				if (ss.peek() == ',')
					ss.ignore();
			}

			for (int it : vect)	//for all observees add them
			{
				this->getEntityByFileID(it)->addObserver(*this->getEntityByFileID(object.attribute("id").as_uint()));
			}
		}*/
	}

	DialogueManager::_Player = this->getPlayer();
}

void ObjectFactorySystem::saveLevel()
{
	pugi::xml_document levelDoc;
	levelDoc.load_file("Assets/Data/Saves/TempObjectData.xml");

	//PlayerData
	pugi::xml_node playerRoot = levelDoc.child("Player");	//get the current level ref
	for (pugi::xml_node item = playerRoot.first_child(); item;)	//remove all player objects
	{
		pugi::xml_node next = item.next_sibling();
		item.parent().remove_child(item);
		item = next;
	}

	Entity* player = this->getPlayer();//make a reference to player

	playerRoot.append_child("Player");
	playerRoot.last_child().append_attribute("positionX").set_value(int(player->get<PositionComponent>()->_position.x));
	playerRoot.last_child().append_attribute("positionY").set_value(int(player->get<PositionComponent>()->_position.y));

	//inventory
	playerRoot.last_child().append_child("Inventory");
	for (auto it = player->get<InventoryComponent>()->_items.begin(); it != player->get<InventoryComponent>()->_items.end(); ++it)
	{
		playerRoot.last_child().child("Inventory").append_child("Item").append_attribute("ScriptTable").set_value((*it)->getScriptTable().c_str());
		playerRoot.last_child().child("Inventory").last_child().append_attribute("ScriptPath").set_value((*it)->getScriptFilePath().c_str());
	}

	//LevelData
	pugi::xml_node levelRoot = levelDoc.child("Cells").find_child_by_attribute("Cell", "CellID", GameManager::_currentCell.c_str());	//get the current level ref

	if (levelRoot.empty())	//if cell ref does not exist in temp file create it
		levelDoc.child("Cells").append_child("Cell").append_attribute("CellID").set_value(GameManager::_currentCell.c_str());

	levelRoot = levelDoc.child("Cells").find_child_by_attribute("Cell", "CellID", GameManager::_currentCell.c_str());	//get the current level ref

	for (pugi::xml_node item = levelRoot.first_child(); item;)	//remove all world objects
	{
		pugi::xml_node next = item.next_sibling();
		item.parent().remove_child(item);
		item = next;
	}
	
	//add world objects
	for (std::vector<std::shared_ptr<Entity>>::iterator i = _entitys.begin(); i != _entitys.end(); ++i)
		if (i->get()->getFileID() != 0)
		{
			levelRoot.append_child((*i)->getName().c_str());
			levelRoot.last_child().append_attribute("fileID").set_value((*i)->getFileID());
			levelRoot.last_child().append_attribute("positionX").set_value(int((*i)->get<PositionComponent>()->_position.x));
			levelRoot.last_child().append_attribute("positionY").set_value(int((*i)->get<PositionComponent>()->_position.y));
		}
		
	levelDoc.save_file("Assets/Data/Saves/TempObjectData.xml");
}

Entity* ObjectFactorySystem::getPlayer()
{
	for (std::vector<std::shared_ptr<Entity>>::iterator it = _entitys.begin(); it != _entitys.end(); ++it)
		if (it->get()->hasTag("Player"))
			return it->get();

	return nullptr;
}

Entity* ObjectFactorySystem::getEntityByFileID(unsigned int fileID)
{
	for (std::vector<std::shared_ptr<Entity>>::iterator it = _entitys.begin(); it != _entitys.end(); ++it)
		if (it->get()->getFileID() == fileID)
			return it->get();

	return nullptr;
}