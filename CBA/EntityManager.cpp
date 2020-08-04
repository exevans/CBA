#include <sstream>

#include "NavMeshManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "TextureManager.h"
#include "LuaManager.h"
#include "GameManager.h"
#include "LuaManager.h"
#include "LuaEntityHandle.h"

//#include <LuaBridge.h>

EntityManager::EntityManager(SystemManager* systemManager, LuaManager* luaManager, NavMeshManager* navMeshManager) : 
	_systemManager(systemManager),
	_luaManager(luaManager),
	_navMeshManager(navMeshManager),
	_entityCounter(0),
	_loadingNewLevel(false),
	_levelToLoad(""),
	_destinationEntId(0)
{

	_luaManager->setEntityManager(this);

	addComponentType<HealthComponent>(ComponentType::Health);
	addComponentType<PositionComponent>(ComponentType::Position);
	addComponentType<MovementComponent>(ComponentType::Movement);
	addComponentType<AIComponent>(ComponentType::AI);
	addComponentType<TextureComponent>(ComponentType::Texture);
	addComponentType<AnimationComponent>(ComponentType::Animation);
	addComponentType<InputComponent>(ComponentType::Input);
	addComponentType<CollisionComponent>(ComponentType::Collision);
	addComponentType<ItemComponent>(ComponentType::Item);
	addComponentType<InventoryComponent>(ComponentType::Inventory);
	addComponentType<SpawnerComponent>(ComponentType::Spawner);
	addComponentType<DialogueComponent>(ComponentType::Dialogue);
	addComponentType<QuestComponent>(ComponentType::Quest);
	addComponentType<ScriptStateMachineComponent>(ComponentType::ScriptState);
	addComponentType<TimerComponent>(ComponentType::Timer);
	addComponentType<InteractComponent>(ComponentType::Interact);
	addComponentType<ScheduleComponent>(ComponentType::Schedule);
	addComponentType<LightingComponent>(ComponentType::Lighting);
}

EntityManager::~EntityManager() {
	std::cout << "started to destryot entity manager" << std::endl;
	//purge handled in state manager
	purge();
}

void EntityManager::requestLoadLevel(const EntityID& entity) {
	_levelToLoad = getEntityInfo(entity).destinationCellId;
	_destinationEntId = getEntityInfo(entity).destinationEntId;
	_loadingNewLevel = true;
}

void EntityManager::requestReloadLevel() {
	_loadingNewLevel = true;
}

void EntityManager::requestLoadLevel(const std::string& path) {
	_levelToLoad = path;
	_loadingNewLevel = true;
}

bool EntityManager::loadingLevel(std::string& path) {
	path = _levelToLoad;
	if (_loadingNewLevel) {
		_loadingNewLevel = false;
		return true;
	}
	return false;
}

void EntityManager::loadLevel(const std::string& path) {
	//save current state of level
	saveLevel();
	//remove all stored entitys
	purge();

	//load level data
	pugi::xml_document doc;
	doc.load_file(path.c_str());

	pugi::xml_node mapRoot = doc.child("map");
	pugi::xml_node objects = mapRoot.find_child_by_attribute("objectgroup", "name", "Entities");

	//load current level data
	pugi::xml_document savedLevelDoc;
	savedLevelDoc.load_file("Assets/Data/Saves/TempObjectData.xml");

	//load current player data
	pugi::xml_node savedPlayerRoot = savedLevelDoc.child("Player").first_child();

	int playerInt = addEntity("Assets/Scripts/Characters.lua", "player");

	if (!savedPlayerRoot.empty())
	{
		//pos should be saved per level
		/*if (GameManager::_triggerId != 0)
			getComponent<PositionComponent>(playerInt, ComponentType::Position)->_position = sf::Vector2f(objects.find_child_by_attribute("object", "id", std::to_string(GameManager::_triggerId).c_str()).attribute("x").as_float() + 15 - getComponent<TextureComponent>(playerInt, ComponentType::Texture)->_textureSize.x / 2, objects.find_child_by_attribute("object", "id", std::to_string(GameManager::_triggerId).c_str()).attribute("y").as_float() - getComponent<TextureComponent>(playerInt, ComponentType::Texture)->_textureSize.y / 2);	//sets the position
		else if (savedPlayerRoot.attribute("positionX"))
			getComponent<PositionComponent>(playerInt, ComponentType::Position)->_position = sf::Vector2f(savedPlayerRoot.attribute("positionX").as_float(), savedPlayerRoot.attribute("positionY").as_float());	//sets the position

		getComponent<InventoryComponent>(playerInt, ComponentType::Inventory)->clearInventory();	//clears the invent so it can be loaded feom file
		pugi::xml_node inventRoot = savedPlayerRoot.child("Inventory");
		for (pugi::xml_node item = inventRoot.first_child(); item; item = item.next_sibling())	//for all saved items in temp file
		{
			int itemId = addEntity(item.attribute("ScriptPath").as_string(), item.attribute("ScriptTable").as_string());
			//getComponent<InventoryComponent>(playerInt, ComponentType::Inventory)->addEntity(std::move(newItemEntity));
		}*/
	}
	else
	{
		/*if (!mapRoot.child("properties").find_child_by_attribute("property", "name", "playerEntrance").empty()) {
			int playerEntranceFileId = mapRoot.child("properties").find_child_by_attribute("property", "name", "playerEntrance").attribute("playerEntrance").as_uint();
		}
		getComponent<PositionComponent>(playerInt, ComponentType::Position)->_position = sf::Vector2f(704.f, 50.f);*/
	}

	//load current level data
	pugi::xml_node savedLevelRoot = savedLevelDoc.child("Cells").find_child_by_attribute("Cell", "CellID", GameManager::_newCell.c_str());

	if (!savedLevelRoot.empty())	//if a temp file exists for the cell
	{
		/*for (pugi::xml_node savedItem = savedLevelRoot.first_child(); savedItem; savedItem = savedItem.next_sibling()) {
			for (pugi::xml_node object = objects.first_child(); object; object = object.next_sibling())
			{
				pugi::xml_node properties = object.child("properties");
				if (object.attribute("id").as_uint() == savedItem.attribute("fileID").as_uint())
				{
					int newEntId = addEntity(properties.find_child_by_attribute("property", "name", "scriptPath").attribute("value").as_string(), properties.find_child_by_attribute("property", "name", "scriptTable").attribute("value").as_string());

					//if (object.attribute("id"))
						//newEntity->setFileID(object.attribute("id").as_uint());	//sets the fileID
					if (object.attribute("x") && getComponent<TextureComponent>(newEntId, ComponentType::Texture) != nullptr)
						getComponent<PositionComponent>(newEntId, ComponentType::Position)->_position = sf::Vector2f((object.attribute("x").as_float()), (object.attribute("y").as_float() - getComponent<TextureComponent>(newEntId, ComponentType::Texture)->_textureSize.y));	//sets the position			
					else if (object.attribute("x") && getComponent<CollisionComponent>(newEntId, ComponentType::Collision) != nullptr)
						getComponent<PositionComponent>(newEntId, ComponentType::Position)->_position = sf::Vector2f(object.attribute("x").as_float(), object.attribute("y").as_float());	//sets the position
					if (properties.find_child_by_attribute("property", "name", "destinationCellId"))
						getComponent<PositionComponent>(newEntId, ComponentType::Position)->_destinationCellId = properties.find_child_by_attribute("property", "name", "destinationCellId").attribute("value").as_string();
					if (properties.find_child_by_attribute("property", "name", "destinationTriggerId"))
						getComponent<PositionComponent>(newEntId, ComponentType::Position)->_destinationTriggerId = properties.find_child_by_attribute("property", "name", "destinationTriggerId").attribute("value").as_uint();

					break;
				}
			}
		}*/
	}
	else
	{
		//load all entitys in for level
		loadLevelEntitys(objects);
		//once all entities loaded
		loadEntityCallbacks(objects);
	}

	//set player start Position based on spawn info
	int playerEntranceFileId = 0;
	if (_destinationEntId != 0) {
		playerEntranceFileId = _destinationEntId;
	}
	else if (!mapRoot.child("properties").find_child_by_attribute("property", "name", "playerEntrance").empty()) {
		//if there's an entrance in the map use it
		playerEntranceFileId = mapRoot.child("properties").find_child_by_attribute("property", "name", "playerEntrance").attribute("value").as_uint();
	}
	EntityID playerEntranceId = getEntityIdFromFileId(playerEntranceFileId);
	sf::Vector2f startPos = getComponent<PositionComponent>(playerInt, ComponentType::Position)->_position = getComponent<PositionComponent>(playerEntranceId, ComponentType::Position)->_position - sf::Vector2f(0, 50);
	getComponent<AIComponent>(playerInt, ComponentType::AI)->_targetPosition = startPos + sf::Vector2f(0, 50);
}

void EntityManager::loadLevelEntitys(const pugi::xml_node& objects) {
	for (pugi::xml_node object = objects.first_child(); object; object = object.next_sibling())
	{
		pugi::xml_node properties = object.child("properties");

		if (properties.find_child_by_attribute("property", "name", "scriptPath").empty()) {
			std::string type = object.attribute("type").as_string();
			if (type == "Route") {
				unsigned int parentFileId = properties.find_child_by_attribute("property", "name", "ParentId").attribute("value").as_uint();

				for (auto& ent : _entities) {
					if (ent.second.first.fileID != parentFileId) { continue; }
					AIComponent* aiComp = getComponent<AIComponent>(ent.first, ComponentType::AI);
					std::string pointsS = object.child("polyline").attribute("points").as_string();
					std::stringstream ss(pointsS);
					float i;
					while (ss >> i)
					{
						sf::Vector2f newPoint(i + object.attribute("x").as_float(), object.attribute("y").as_float());

						if (ss.peek() == ',' || ss.peek() == ' ')
							ss.ignore();

						ss >> i;

						newPoint.y += i;

						aiComp->_patrolPositions.push_back(newPoint);
					}
				}
			}
			continue;
		}

		EntityID newEntId = addEntity(properties.find_child_by_attribute("property", "name", "scriptPath").attribute("value").as_string(), properties.find_child_by_attribute("property", "name", "scriptTable").attribute("value").as_string());

		//ensures valid enty is created
		if (newEntId == -1) { continue; }

		if (object.attribute("id"))
			_entities.find(newEntId)->second.first.fileID = object.attribute("id").as_uint();	//sets the fileID
		if (object.attribute("x") && getComponent<TextureComponent>(newEntId, ComponentType::Texture) != nullptr)
			getComponent<PositionComponent>(newEntId, ComponentType::Position)->_position = sf::Vector2f((object.attribute("x").as_float() + getComponent<TextureComponent>(newEntId, ComponentType::Texture)->_sprite.getGlobalBounds().width / 2), (object.attribute("y").as_float()/* + getComponent<TextureComponent>(newEntId, ComponentType::Texture)->_sprite.getGlobalBounds().height/2*/));	//sets the position			
		else if (object.attribute("x") && getComponent<CollisionComponent>(newEntId, ComponentType::Collision) != nullptr)
			getComponent<PositionComponent>(newEntId, ComponentType::Position)->_position = sf::Vector2f(object.attribute("x").as_float() + getComponent<CollisionComponent>(newEntId, ComponentType::Collision)->_colRect.width / 2, object.attribute("y").as_float() + getComponent<CollisionComponent>(newEntId, ComponentType::Collision)->_colRect.height);	//sets the position
		else if (object.attribute("x") && getComponent<InteractComponent>(newEntId, ComponentType::Interact) != nullptr)
			getComponent<PositionComponent>(newEntId, ComponentType::Position)->_position = sf::Vector2f(object.attribute("x").as_float() + getComponent<InteractComponent>(newEntId, ComponentType::Interact)->_interactRect.width / 2, object.attribute("y").as_float() + getComponent<InteractComponent>(newEntId, ComponentType::Interact)->_interactRect.height);	//sets the position
		if (properties.find_child_by_attribute("property", "name", "conversationId")) {
			int conversationId = properties.find_child_by_attribute("property", "name", "conversationId").attribute("value").as_uint();
			getComponent<DialogueComponent>(newEntId, ComponentType::Dialogue)->_baseConversationID = conversationId;
		}

		if (properties.find_child_by_attribute("property", "name", "destinationCellId")) {
			std::string cellId = properties.find_child_by_attribute("property", "name", "destinationCellId").attribute("value").as_string();
			EntityID entFileId = properties.find_child_by_attribute("property", "name", "destinationEntId").attribute("value").as_uint();
			_entities.find(newEntId)->second.first.destinationCellId = cellId;
			_entities.find(newEntId)->second.first.destinationEntId = entFileId;
		}
		//getComponent<PositionComponent>(newEntId, ComponentType::Position)->_destinationCellId = properties.find_child_by_attribute("property", "name", "destinationCellId").attribute("value").as_string();
		if (properties.find_child_by_attribute("property", "name", "destinationTriggerId"))
			getComponent<PositionComponent>(newEntId, ComponentType::Position)->_destinationTriggerId = properties.find_child_by_attribute("property", "name", "destinationTriggerId").attribute("value").as_uint();
	}
}

void EntityManager::loadEntityCallbacks(const pugi::xml_node& objects) {
	for (pugi::xml_node object = objects.first_child(); object; object = object.next_sibling()) {
		pugi::xml_node properties = object.child("properties");

		if (properties.find_child_by_attribute("property", "name", "observationCallback")) {
			std::string observationInfo = properties.find_child_by_attribute("property", "name", "observationCallback").attribute("value").as_string();
			std::stringstream ss(observationInfo);

			int observingId;
			ss >> observingId;

			std::string observedEvent;
			ss >> observedEvent;

			std::string luaCallback;
			ss >> luaCallback;
			EntityID actualId = getEntityIdFromFileId(observingId);
			if (actualId == -1) { return; }

			_luaManager->subscribeToLuaEvent(actualId, getEntityIdFromFileId(object.attribute("id").as_uint()), observedEvent, luaCallback);
		}
	}
}

void EntityManager::saveLevel() {

}

LuaManager* EntityManager::getLuaManager() {
	return _luaManager;
}

NavMeshManager* EntityManager::getNavMeshManager() {
	return _navMeshManager;
}

int EntityManager::addEntity() {
	EntityID entityId = _entityCounter;
	//check added successfully
	if (!_entities.emplace(entityId, EntityData(EntityInfo(), ComponentContainer())).second) {
		return -1;
	}

	std::cout << "entity Added, " << _entities.size() << " entites" << std::endl;
	//increase counter for next entity
	++_entityCounter;

	//send event to say entity spawned
	_systemManager->addEvent(entityId, (EventID)EntityEvent::Spawned);

	//add entity handle
	_luaEntityHandle.push_back(entityId/*, new LuaEntityHandle(this, entityId)*/);
	
	//add handle for lua
	sol::function createHandle = _luaManager->getLuaState()["createHandle"];
	if (createHandle.valid()) {
		createHandle(LuaEntityHandle(this, entityId));
	}

	return entityId;
}

int EntityManager::addEntity(const std::string& scriptFileName, const std::string& scriptTable) {
	int entId = -1;

	entId = addEntity();
	if (entId == -1) {
		return -1;
	}

	_entities.find(entId)->second.first.scriptPath = scriptFileName;
	_entities.find(entId)->second.first.scriptTable = scriptTable;

	//load the entitys script
	_luaManager->loadEntityLua(entId, scriptFileName, scriptTable);
	sol::table table = _luaManager->getLuaState()[scriptTable];
	if (!table.empty()) {
		if (table["name"] != sol::nil) {
			_entities.find(entId)->second.first.name = table["name"];
		}
		if (table["Tags"] != sol::nil)
		{
			sol::table tagsTable = table["Tags"];

			for (int i = 0; i < tagsTable.size(); ++i)
			{
				std::string elementRef = tagsTable[i + 1];
				//add tag to tags vector
				_entities.find(entId)->second.first.tags.push_back(elementRef);
				//std::cout << "New tag added: " << _tags[_tags.size() - 1] << std::endl;
			}
		}

		sol::table compTable = table["HealthComponent"];
		if (compTable != sol::nil) {
			addComponent(entId, ComponentType::Health);
			getComponent<HealthComponent>(entId, ComponentType::Health)->readIn(compTable);
		}
		if (table["PositionComponent"] != sol::nil) {
			addComponent(entId, ComponentType::Position);
		}
		compTable = table["MovementComponent"];
		if (compTable != sol::nil) {
			addComponent(entId, ComponentType::Movement);
			getComponent<MovementComponent>(entId, ComponentType::Movement)->readIn(compTable);
		}
		compTable = table["TextureComponent"];
		if (compTable != sol::nil) {
			addComponent(entId, ComponentType::Texture);
			getComponent<TextureComponent>(entId, ComponentType::Texture)->readIn(compTable);
		}
		compTable = table["CollisionComponent"];
		if (compTable != sol::nil) {
			addComponent(entId, ComponentType::Collision);

			PositionComponent* posComp = getComponent<PositionComponent>(entId, ComponentType::Position);
			TextureComponent* texComp = getComponent<TextureComponent>(entId, ComponentType::Texture);
			getComponent<CollisionComponent>(entId, ComponentType::Collision)->readIn(compTable, posComp, texComp);
		}
		compTable = table["InteractComponent"];
		if (compTable != sol::nil) {
			addComponent(entId, ComponentType::Interact);

			PositionComponent* posComp = getComponent<PositionComponent>(entId, ComponentType::Position);
			TextureComponent* texComp = getComponent<TextureComponent>(entId, ComponentType::Texture);
			getComponent<InteractComponent>(entId, ComponentType::Interact)->readIn(compTable, posComp, texComp);
		}
		compTable = table["InventoryComponent"];
		if (table[compTable] != sol::nil) {
			addComponent(entId, ComponentType::Inventory);
			getComponent<InventoryComponent>(entId, ComponentType::Inventory)->readIn(compTable);
		}
		if (table["InputComponent"] != sol::nil) {
			addComponent(entId, ComponentType::Input);
		}
		compTable = table["AnimationComponent"];
		if (compTable != sol::nil) {
			addComponent(entId, ComponentType::Animation);
			AnimationComponent* aniComp = getComponent<AnimationComponent>(entId, ComponentType::Animation);
			TextureComponent* texComp = getComponent<TextureComponent>(entId, ComponentType::Texture);

			//aniComp->_currentTexPos = sf::Vector2u(texComp->_sprite.getTextureRect().left, texComp->_sprite.getTextureRect().top);
			//aniComp->_imageSize = TextureManager::getTextureRef(texComp->_texturePath).getSize();
			//aniComp->_animationRate = compTable["textureRate"];

			compTable = table["AnimationComponent"];
			aniComp->readIn(compTable);
			//texComp->_sprite.setOrigin(texComp->_sprite.getPosition().x + aniComp->_spriteSize.x / 2, texComp->_sprite.getPosition().y + aniComp->_spriteSize.y);	//set origin to centre of texture
		}
		if (table["ItemComponent"] != sol::nil) {
			addComponent(entId, ComponentType::Item);
		}
		if (table["SpawnerComponent"] != sol::nil) {
			addComponent(entId, ComponentType::Spawner);
		}
		compTable = table["AIComponent"];
		if (compTable != sol::nil)
		{
			addComponent(entId, ComponentType::AI);
			getComponent<AIComponent>(entId, ComponentType::AI)->readIn(compTable);
		}
		compTable = table["DialogueComponent"];
		if (compTable != sol::nil) {
			addComponent(entId, ComponentType::Dialogue);
			getComponent<DialogueComponent>(entId, ComponentType::Dialogue)->readIn(compTable);
		}
		compTable = table["QuestComponent"];
		if (compTable != sol::nil) {
			addComponent(entId, ComponentType::Quest);
			getComponent<QuestComponent>(entId, ComponentType::Quest)->readIn(compTable);
		}
		if (table["ScriptStateMachineComponent"] != sol::nil) {
			addComponent(entId, ComponentType::ScriptState);
		}
		if (table["ScheduleComponent"] != sol::nil) {
			addComponent(entId, ComponentType::Schedule);
		}
		compTable = table["TimerComponent"];
		if (compTable != sol::nil) {
			addComponent(entId, ComponentType::Timer);
			getComponent<TimerComponent>(entId, ComponentType::Timer)->readIn(compTable);
		}
		compTable = table["LightingComponent"];
		if (compTable != sol::nil) {
			addComponent(entId, ComponentType::Lighting);
			getComponent<LightingComponent>(entId, ComponentType::Lighting)->readIn(compTable, getComponent<PositionComponent>(entId, ComponentType::Position), this->_systemManager->getShared()->_lightManager, entId);
		}
		// call init function
		sol::function initFunc = table["init"];
		if (initFunc != sol::nil) {
			initFunc(entId);
		}
	}

	_Uint32t newBitset = _entities.find(entId)->second.first.bitset;
	_systemManager->entityModified(entId, newBitset);

	return entId;
}

bool EntityManager::removeEntity(const EntityID& entId) {
	auto itr = _entities.find(entId);
	if (itr == _entities.end()) { return false; };

	_systemManager->addEvent(entId, (EventID)EntityEvent::Despawned);

	//remove lua handle
	sol::function removeHandle = _luaManager->getLuaState()["removeHandle"];
	if (removeHandle.valid()) {
		removeHandle(entId);
	}

	//delete the handle pointer
	//delete _luaEntityHandle.at(entId);
	//_luaEntityHandle.at(entId) = nullptr;

	//remove entity handle
	for (size_t it = 0; it < _luaEntityHandle.size(); ++it) {
		if (_luaEntityHandle[it] == entId) {
			_luaEntityHandle.erase(_luaEntityHandle.begin() + it);
		}
	}

	//deletes all components of entity
	while (itr->second.second.begin() != itr->second.second.end()) {
		delete itr->second.second.back();
		itr->second.second.pop_back();
	}
	_entities.erase(entId);
	_systemManager->removeEntity(entId);
	//_luaManager->removeEntityFromObservers(entId);
	_luaManager->removeEntity(entId);

	--_entityCounter;
	return true;
}

bool EntityManager::addComponent(const EntityID& entID, const ComponentType& comp) {
	//entity must exist
	auto itr = _entities.find(entID);
	if (itr == _entities.end()) {
		return false;
	}
	//component must not already exist
	if (((itr->second.first.bitset) & (1 << unsigned int(comp))) != 0) {
		return false;
	}

	//componentType must be in factory
	auto itr2 = _componentFactory.find(comp);
	if (itr2 == _componentFactory.end()) {
		std::cout << "Component type doesn't exist in factory" << std::endl;
		return false;
	}

	//create component from factory
	Component* component = itr2->second();
	itr->second.second.emplace_back(component);

	itr->second.first.bitset = itr->second.first.bitset | (1 << unsigned int(comp));
}

bool EntityManager::hasComponent(const EntityID& ent, const ComponentType& comp) {
	auto itr = _entities.find(ent);
	if (itr == _entities.end()) { return false; }

	_Uint32t checker = (1 << unsigned int(comp));
	if ((itr->second.first.bitset & checker) == checker) {
		return true;
	}

	return false;
}

bool EntityManager::hasEntity(const EntityID& ent) {
	auto itr = _entities.find(ent);
	if (itr == _entities.end()) { return false; }

	return true;
}

void EntityManager::purge() {
	_systemManager->purgeEntities();

	sol::function removeHandle = _luaManager->getLuaState()["removeHandle"];
	for (auto& handle : _luaEntityHandle) {
		if (removeHandle.valid()) {
			removeHandle(handle);
		}

		//delete pointer
		//delete _luaEntityHandle.at(handle.first);
		//_luaEntityHandle.at(handle.first) = nullptr;
	}
	
	_luaEntityHandle.clear();
	//_luaManager->purgeScripts();

	//needs to be changed to delete comp pointers
	for (auto& entity : _entities) {
		for (auto& component : entity.second.second) {
			delete component;
			component = nullptr;
		}
		entity.second.second.clear();
	}
	_entities.clear();

	_entityCounter = 0;
}

const EntityID & EntityManager::getEntityIdFromFileId(unsigned int fileId)
{
	for (auto& ent : _entities) {
		if (ent.second.first.fileID == fileId)
			return ent.first;
	}
	return 0;
}
