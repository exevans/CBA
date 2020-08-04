#include <memory>
#include "LuaEntityHandle.h"
#include "EntityManager.h"
#include "LuaManager.h"
#include "Entity.h"

LuaEntityHandle::LuaEntityHandle(EntityManager* entManag, const EntityID baseEnt) : _e(baseEnt), _entityManager(entManag)
{
	
}

EntityID LuaEntityHandle::getId() 
{
	return _e;
}

void LuaEntityHandle::addScheduleTask(const std::string& taskName) {
	if (_entityManager->hasComponent(_e, ComponentType::Schedule)) {
		_entityManager->getComponent<ScheduleComponent>(_e, ComponentType::Schedule)->_tasks.emplace_back(taskName, ScheduleData());
	}
	else
	{
		std::cerr << "Entity doesn't have a sched Component" << std::endl;
	}
}

void LuaEntityHandle::addScheduleTaskInt(const std::string& taskName, int data) {
	if (_entityManager->hasComponent(_e, ComponentType::Schedule)) {
		_entityManager->getComponent<ScheduleComponent>(_e, ComponentType::Schedule)->_tasks.emplace_back(taskName, ScheduleData(data));
	}
	else
	{
		std::cerr << "Entity doesn't have a sched Component" << std::endl;
	}
}

void LuaEntityHandle::addScheduleTaskStr(const std::string& taskName, std::string& data) {
	if (_entityManager->hasComponent(_e, ComponentType::Schedule)) {
		_entityManager->getComponent<ScheduleComponent>(_e, ComponentType::Schedule)->_tasks.emplace_back(taskName, ScheduleData(data));
	}
	else
	{
		std::cerr << "Entity doesn't have a sched Component" << std::endl;
	}
}

void LuaEntityHandle::addScheduleTask2f(const std::string& taskName, sf::Vector2f& data) {
	if (_entityManager->hasComponent(_e, ComponentType::Schedule)) {
		_entityManager->getComponent<ScheduleComponent>(_e, ComponentType::Schedule)->_tasks.emplace_back(taskName, ScheduleData(data));
	}
	else
	{
		std::cerr << "Entity doesn't have a sched Component" << std::endl;
	}
}

void LuaEntityHandle::clearSchedule() {
	if (_entityManager->hasComponent(_e, ComponentType::Schedule)) {
		_entityManager->getComponent<ScheduleComponent>(_e, ComponentType::Schedule)->_tasks.clear();
		_entityManager->getComponent<ScheduleComponent>(_e, ComponentType::Schedule)->_currentTaskIndex = 0;
		_entityManager->getComponent<ScheduleComponent>(_e, ComponentType::Schedule)->_changingTask = true;
	}
	else
	{
		std::cerr << "Entity doesn't have a sched Component" << std::endl;
	}
}

void LuaEntityHandle::startTimer(const std::string& timerName) {
	if (_entityManager->hasComponent(_e, ComponentType::Timer)) {
		TimerComponent* timerComp = _entityManager->getComponent<TimerComponent>(_e, ComponentType::Timer);
		timerComp->_timers[timerName]._active = true;
		timerComp->_timers[timerName]._elapsedTime = 0;
	}
	else
	{
		std::cerr << "Entity doesn't have a timer Component" << std::endl;
	}
}

void LuaEntityHandle::stopTimer(const std::string& timerName) {
	if (_entityManager->hasComponent(_e, ComponentType::Timer)) {
		TimerComponent* timerComp = _entityManager->getComponent<TimerComponent>(_e, ComponentType::Timer);
		timerComp->_timers[timerName]._active = false;
	}
	else
	{
		std::cerr << "Entity doesn't have a timer Component" << std::endl;
	}
}

void LuaEntityHandle::setControllable(bool newVal) {
	if (_entityManager->hasComponent(_e, ComponentType::Input)) {
		_entityManager->getComponent<InputComponent>(_e, ComponentType::Input)->_controllable = newVal;
		std::cout << "Entity Comtrollable: " << newVal << std::endl;
	}
	else
	{
		std::cerr << "Entity doesn't have a input Component" << std::endl;
	}
}

void LuaEntityHandle::toggleNoclip() {
	if (_entityManager->hasComponent(_e, ComponentType::Collision)) {
		CollisionComponent* colComp = _entityManager->getComponent<CollisionComponent>(_e, ComponentType::Collision);
		colComp->setActive(!colComp->isActive());
		std::cout << "Entity Collision: " << colComp->isActive() << std::endl;
	}
	else
	{
		std::cerr << "Entity doesn't have a collision Component" << std::endl;
	}
}

void LuaEntityHandle::setInMovementToEnt(const unsigned int targetEntId) {
	if (_entityManager->hasComponent(_e, ComponentType::Position) && _entityManager->hasComponent(targetEntId, ComponentType::Position) && _entityManager->hasComponent(_e, ComponentType::Movement)) {
		sf::Vector2f targetPos = _entityManager->getComponent<PositionComponent>(targetEntId, ComponentType::Position)->_position;
		sf::Vector2f currentPos = _entityManager->getComponent<PositionComponent>(_e, ComponentType::Position)->_position;
		sf::Vector2f direction = targetPos - currentPos;

		_entityManager->getComponent<MovementComponent>(_e, ComponentType::Movement)->setVelocity(direction);
	}
	else {
		std::cerr << "Entity doesn't have correct Components" << std::endl;
	}
}

bool LuaEntityHandle::hasTag(std::string checkTag) const
{
	return _entityManager->getEntityInfo(_e).containsTag(checkTag);
}

void LuaEntityHandle::setHp(const unsigned int newHp) 
{
	if (_entityManager->hasComponent(_e, ComponentType::Health)) {
		_entityManager->getComponent<HealthComponent>(_e, ComponentType::Health)->_health = newHp;
	}
	else
	{
		std::cerr << "Entity doesn't have a health Component" << std::endl;
	}
}

void LuaEntityHandle::addItemToInventory(LuaEntityHandle* item)
{
	/*if (_e->hasComponent(std::type_index(typeid(InventoryComponent))))
	{
		std::cout << "New item " << item->getParentEntity()->getName() << " added to inventory" << std::endl;
		std::shared_ptr<Entity> itemP(new Entity());
		(*itemP.get()) = (*item->getParentEntity());
		_e->get<InventoryComponent>()->addEntity(std::move(itemP));
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have an inventory component" << std::endl;
	}*/
}

void LuaEntityHandle::addContentsToInventory(LuaEntityHandle* otherContainer)
{
	/*if (_e->hasComponent(std::type_index(typeid(InventoryComponent))) && otherContainer->getParentEntity()->hasComponent(std::type_index(typeid(InventoryComponent))))
	{
		auto containerInvent = otherContainer->getParentEntity()->get<InventoryComponent>();

		if (containerInvent->_items.size() > 0)
		{
			auto mainInvent = _e->get<InventoryComponent>();

			for (size_t i = 0; i < containerInvent->_items.size(); i++)
				mainInvent->addEntity(containerInvent->_items[i]);

			containerInvent->clearInventory();
		}
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have an inventory component" << std::endl;
	}*/
}

bool LuaEntityHandle::isInventoryFull() const
{
	/*if (_e->hasComponent(std::type_index(typeid(InventoryComponent))))
	{
		return _e->get<InventoryComponent>()->isInventoryFull();
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have an inventory component" << std::endl;
	}*/

	return true;
}

bool LuaEntityHandle::isInventoryEmpty() const
{
	/*if (_e->hasComponent(std::type_index(typeid(InventoryComponent))))
	{
		if (_e->get<InventoryComponent>()->_items.size() == 0)
			return true;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have an inventory component" << std::endl;
	}*/

	return false;
}

void LuaEntityHandle::removeItemFromInventory(LuaEntityHandle* LuaEntityHandle)
{
	/*if (_e->hasComponent(std::type_index(typeid(InventoryComponent))))
	{
		unsigned int itemId = LuaEntityHandle->getParentEntity()->getID();
		_e->get<InventoryComponent>()->removeEntity(itemId);
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have an inventory component" << std::endl;
	}*/
}

void LuaEntityHandle::damage(unsigned int dmg)
{
	if (_entityManager->hasComponent(_e, ComponentType::Health))
	{
		HealthComponent* hlthComp = _entityManager->getComponent<HealthComponent>(_e, ComponentType::Health);
		hlthComp->_health -= dmg;
		std::cout << _e << " damaged for " << dmg << std::endl;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have an Health component" << std::endl;
	}
}

void LuaEntityHandle::heal(unsigned int hp)
{
	if (_entityManager->hasComponent(_e, ComponentType::Health))
	{
		HealthComponent* hlthComp = _entityManager->getComponent<HealthComponent>(_e, ComponentType::Health);
		hlthComp->_health += hp;
		std::cout << _e << " healed for " << hp << std::endl;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have an Health component" << std::endl;
	}
}

/*LuaEntityHandle LuaEntityHandle::spawn(std::string& path, std::string& table) {
	int newEntId = _entityManager->addEntity(path, table);
	return LuaEntityHandle(_entityManager, newEntId);
}*/

/*void LuaEntityHandle::spawnChild(std::string tableName)
{
	/*if (_e->hasComponent(std::type_index(typeid(SpawnerComponent))))
	{
		auto spawnComp = _e->get<SpawnerComponent>();

		spawnComp->_spawnTableName = tableName;
		spawnComp->_spawning = true;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a Spawn component" << std::endl;
	}
}*/

void LuaEntityHandle::moveTowardsPosition(sf::Vector2f targetPos)	//NEEDS WORK
{
	if (_entityManager->hasComponent(_e, ComponentType::Movement))
	{
		CollisionComponent* colComp = _entityManager->getComponent<CollisionComponent>(_e, ComponentType::Collision);
		MovementComponent* movComp = _entityManager->getComponent<MovementComponent>(_e, ComponentType::Movement);
		sf::Vector2f position = sf::Vector2f(colComp->_colRect.left + colComp->_colRect.width / 2, colComp->_colRect.top + colComp->_colRect.height / 2);

		//if (this->getDistanceBetweenPositions(targetPos, sf::Vector2f(colComp->_colRect.left + colComp->_colRect.width / 2, colComp->_colRect.top + colComp->_colRect.height / 2)) > 5)
		sf::Vector2f dirVect = targetPos - position;
		movComp->accelerate(sf::Vector2f(dirVect.x * movComp->_speed.x, dirVect.y * movComp->_speed.y));
		//else
			//_e->get<MovementComponent>()->_movement = sf::Vector2f(0, 0);
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a Movement component" << std::endl;
	}
}

void LuaEntityHandle::moveOutOfIntersection(const EntityID& staticObjId)
{
	if (_entityManager->hasComponent(_e, ComponentType::Position) && _entityManager->hasComponent(_e, ComponentType::Collision) && _entityManager->hasComponent(staticObjId, ComponentType::Position) && _entityManager->hasComponent(staticObjId, ComponentType::Collision))
	{
		auto m_posComp = _entityManager->getComponent<PositionComponent>(_e, ComponentType::Position);
		auto m_colComp = _entityManager->getComponent<CollisionComponent>(_e, ComponentType::Collision);
		auto s_posComp = _entityManager->getComponent<PositionComponent>(staticObjId, ComponentType::Position);
		auto s_colComp = _entityManager->getComponent<CollisionComponent>(staticObjId, ComponentType::Collision);

		sf::FloatRect area;

		if (s_colComp->_colRect.intersects(m_colComp->_colRect, area))
		{
			if (area.height > area.width)	//left/right
			{
				if (area.contains(s_colComp->_colRect.left, area.top))//left
					m_posComp->_position.x = s_colComp->_colRect.left - m_colComp->_localColRectOffset.x - m_colComp->_colRect.width/2;
				else
					m_posComp->_position.x = s_colComp->_colRect.left + s_colComp->_colRect.width - m_colComp->_localColRectOffset.x + m_colComp->_colRect.width/2;
			}
			else//Top/bottom
			{
				if (area.contains(area.left, s_colComp->_colRect.top))//top
					m_posComp->_position.y = s_colComp->_colRect.top - m_colComp->_localColRectOffset.y/* - m_colComp->_colRect.height*/;
				else
					m_posComp->_position.y = s_colComp->_colRect.top + s_colComp->_colRect.height - m_colComp->_localColRectOffset.y + m_colComp->_colRect.height;
			}

			//sets to not moving
			//m_posComp->getOwnerEntity()->get<MovementComponent>()->_moving = false;
			//set to not colliding id
			m_colComp->collidedEntityID = -1;
		}
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a Position or Collision component" << std::endl;
	}
}

void LuaEntityHandle::setRouteByNavMesh(sf::Vector2f targetPos)
{
	if (_entityManager->hasComponent(_e, ComponentType::Movement) && _entityManager->hasComponent(_e, ComponentType::AI))
	{
		AIComponent* aiComp = _entityManager->getComponent<AIComponent>(_e, ComponentType::AI);
		aiComp->_targetPosition = targetPos;
		//aiComp->calculatingRoot = true;
		_entityManager->getNavMeshManager()->getRouteToTarget(_e);

		//advance schedule
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a Movement or AI component" << std::endl;
	}
}

void LuaEntityHandle::outputText(std::string text)
{
	std::cout << _e << ": " << text << std::endl;
}

float LuaEntityHandle::getDistanceToPosition(sf::Vector2f otherEntPos)
{
	/*if (_e->hasComponent(std::type_index(typeid(PositionComponent))))
	{
		sf::Vector2f pos1 = _e->get<PositionComponent>()->_position;

		sf::Vector2f dist;
		dist = otherEntPos - pos1;

		float totalDist = sqrt((dist.x * dist.x) + (dist.y * dist.y));

		return totalDist;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a Position component" << std::endl;
		return 0.f;
	}*/
	return 0.0;
}

float LuaEntityHandle::getDistanceBetweenPositions(sf::Vector2f firstEntPos, sf::Vector2f otherEntPos)
{
	sf::Vector2f dist;
	dist = otherEntPos - firstEntPos;

	float totalDist = sqrt((dist.x * dist.x) + (dist.y * dist.y));

	return totalDist;
}

void LuaEntityHandle::setEnemyEntity(LuaEntityHandle* otherEnt)
{
	/*if (_e->hasComponent(std::type_index(typeid(AIComponent))))
	{
		auto aiComp = _e->get<AIComponent>();

		//check if enemy already set
		if (aiComp->_enemy == nullptr || (!otherEnt->getParentEntity()->hasParent() && aiComp->_enemy->getID() != otherEnt->getParentEntity()->getID()) || (otherEnt->getParentEntity()->hasParent() && aiComp->_enemy->getID() != otherEnt->getParentEntity()->getParent()->getID()))
		{
			if (otherEnt->getParentEntity()->hasParent())
			{
				aiComp->_enemy = std::make_shared<Entity>(*otherEnt->getParentEntity()->getParent());
				std::cout << _e->getName() << " has enemy " << otherEnt->getParentEntity()->getParent()->getName() << std::endl;
			}
			else 
				aiComp->_enemy = std::make_shared<Entity>(*otherEnt->getParentEntity());

			aiComp->_enemy->addObserver(*_e);
			std::cout << "new enemy" << aiComp->_enemy->getName() << std::endl;
		}
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a AI component" << std::endl;
	}*/
}

/*void LuaEntityHandle::startDialogue(LuaEntityHandle* otherChar)
{
	if (_entityManager->hasComponent(_e, ComponentType::Dialogue) && _entityManager->hasComponent(otherChar->_e, ComponentType::Dialogue)) {
		//GameManager::startConversation(otherChar->getParentEntity());// new way

		//_e->get<DialogueComponent>()->_newConversation = true;
		_entityManager->getComponent<DialogueComponent>(otherChar->_e, ComponentType::Dialogue)->_inConversation = true;
		_entityManager->getComponent<DialogueComponent>(otherChar->_e, ComponentType::Dialogue)->_otherEntity = _e;
		_entityManager->getComponent<DialogueComponent>(_e, ComponentType::Dialogue)->_inConversation = true;
		_entityManager->getComponent<DialogueComponent>(_e, ComponentType::Dialogue)->_otherEntity = otherChar->_e;
		//_e->get<DialogueComponent>()->_activeConversationID = otherChar->getParentEntity()->get<DialogueComponent>()->_baseConversationID;

		//new temp
		//otherChar->getParentEntity()->get<DialogueComponent>()->_inDialogue = true;
		//_e->get<DialogueComponent>()->_otherChar = otherChar->getParentEntity();
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a Dialogue component" << std::endl;
	}
}*/

void LuaEntityHandle::setInactive()
{
	_entityManager->getEntityInfo(_e).active = false;
	//_entityManager->removeEntity(_e);
}

void LuaEntityHandle::setScriptState(std::string newState)
{
	if (_entityManager->hasComponent(_e, ComponentType::ScriptState))
	{
		_entityManager->getComponent<ScriptStateMachineComponent>(_e, ComponentType::ScriptState)->_newState = newState;
		_entityManager->getComponent<ScriptStateMachineComponent>(_e, ComponentType::ScriptState)->_changingState = true;
	}
	else
	{
		std::cout << "Entity dosnt have a script state Machine" << std::endl;
	}
}

std::string LuaEntityHandle::getScriptState()
{
	if (_entityManager->hasComponent(_e, ComponentType::ScriptState))
	{
		return _entityManager->getComponent<ScriptStateMachineComponent>(_e, ComponentType::ScriptState)->_currentState;
	}
	else
	{
		std::cout << "Entity dosnt have a script state Machine" << std::endl;
		return " ";
	}
}

void LuaEntityHandle::setAnimation(std::string animationName, bool looping, bool play)
{
	AnimationComponent* aniComp = _entityManager->getComponent<AnimationComponent>(_e, ComponentType::Animation);
	aniComp->_spriteSheet.setAnimation(animationName, looping, play);

	/*luabridge::LuaRef entityTable = _entityManager->getLuaManager()->getEntityLuaRef(_e);

	//luabridge::LuaRef entityTable = luabridge::getGlobal(_entityManager->getLuaManager()->getLuaState(), _entityManager->getEntityInfo(_e).scriptTable.c_str());
	luabridge::LuaRef graphicsTable = entityTable["TextureComponent"];

	if (graphicsTable["animations"].isTable())
	{
		luabridge::LuaRef animationsTable = graphicsTable["animations"];

		if (animationsTable[animationName].isTable())
		{
			luabridge::LuaRef animationTable = animationsTable[animationName];
			if (animationTable["frame"].isTable())	//single frame
			{
				luabridge::LuaRef frameTable = animationTable["frame"];
				_entityManager->getComponent<TextureComponent>(_e, ComponentType::Texture)->_sprite.setTextureRect(sf::IntRect(frameTable[1], frameTable[2], frameTable[3], frameTable[4]));
			}
			else if (animationTable["startFrame"].isNumber() && animationTable["endFrame"].isNumber())	//multiple frames
			{
				aniComp->playAnimation(animationTable, true);
				/*luabridge::LuaRef frameTable = animationTable["startFrame"];

				if (aniComp->_currentTexPos.y != frameTable[2].cast<unsigned int>())
				{
					_entityManager->getComponent<AnimationComponent>(_e, ComponentType::Animation)->_currentTexPos.y = frameTable[2].cast<unsigned int>();
					_entityManager->getComponent<TextureComponent>(_e, ComponentType::Texture)->_sprite.setTextureRect(sf::IntRect(frameTable[1], frameTable[2], frameTable[3], frameTable[4]));
				}end here
			}
		}
		else
		{
			std::cout << animationName << " is not a valid animation name" << std::endl;
		}
	}*/
}

void LuaEntityHandle::setCollision(sf::Vector2f position, int localPosX, int localPosY, int sizeX, int sizeY)
{
	/*auto colComp = _e->get<CollisionComponent>();

	sf::Vector2f localPos(localPosX, localPosY);

	colComp->_colRect.left = position.x + localPos.x;
	colComp->_colRect.top = position.y + localPos.y;
	colComp->_colRect.width = sizeX;
	colComp->_colRect.height = sizeY;

	colComp->_localColRectOffset = localPos;*/
}

void LuaEntityHandle::activateItem()
{
	/*auto invent = _e->get<InventoryComponent>();
	Entity* equipedItem = invent->getEquipedItem();

	luabridge::LuaRef table = luabridge::getGlobal(L, equipedItem->getScriptTable().c_str());
	luabridge::LuaRef colTable = table["ItemComponent"];
	luabridge::LuaRef activateFunc = colTable["activate"];
	if (activateFunc.isFunction())
		activateFunc(_e->getLuaEntityHandle());*/
}

void LuaEntityHandle::patrol()
{
	AIComponent* aiComp = _entityManager->getComponent<AIComponent>(_e, ComponentType::AI);
	CollisionComponent* colComp = _entityManager->getComponent<CollisionComponent>(_e, ComponentType::Collision);

	if (aiComp->_currentPatrolPoint < aiComp->_patrolPositions.size())
	{
		if (this->getDistanceBetweenPositions(aiComp->_patrolPositions[aiComp->_currentPatrolPoint], sf::Vector2f(colComp->_colRect.left + colComp->_colRect.width / 2, colComp->_colRect.top + colComp->_colRect.height / 2)) < 40)
		{
			aiComp->_currentPatrolPoint++;
			aiComp->_firstPointCalculated = false;
			if (aiComp->_patrolPositions.size() == aiComp->_currentPatrolPoint) {
				_entityManager->getComponent<ScheduleComponent>(_e, ComponentType::Schedule)->advanceCurrentTask();
				aiComp->_currentPatrolPoint = 0;
			}
		}
		
		if (aiComp->_firstPointCalculated == false)
		{
			aiComp->_firstPointCalculated = true;
			this->setRouteByNavMesh(aiComp->_patrolPositions[aiComp->_currentPatrolPoint]);	//for first time
		}
	}
	else {
		_entityManager->getComponent<ScheduleComponent>(_e, ComponentType::Schedule)->endSchedule();
	}
}

void LuaEntityHandle::equipWeapon()
{
	/*if (_e->hasComponent(std::type_index(typeid(InventoryComponent))))
	{
		auto inventComp(_e->get<InventoryComponent>());

		if (inventComp->_items.size() > 0)
		{
			bool newWeaponNeeded = false;
			if (inventComp->getEquipedItem()->hasTag("Weapon"))	//not already weapon equiped
			{
				if (this->getDistanceToPosition(_e->get<AIComponent>()->_enemy->get<PositionComponent>()->_position) > 100)
				{
					if (!inventComp->getEquipedItem()->hasTag("ProjectileLauncher"))
						newWeaponNeeded = true;
				}
				else
				{
					if (!inventComp->getEquipedItem()->hasTag("Melee"))
						newWeaponNeeded = true;
				}
			}

			if (newWeaponNeeded)
				for (auto& it : inventComp->_items)
				{
					auto itemComp(it->get<ItemComponent>());

					if (it->hasTag("Weapon"))
					{
						if (this->getDistanceToPosition(_e->get<AIComponent>()->_enemy->get<PositionComponent>()->_position) > 200)	//weapon type
						{
							if (it->hasTag("ProjectileLauncher"))
							{
								inventComp->setEquipedItemID(it->getID());
								std::cout << "AI weapon equiped " << it->getName() << std::endl;
								break;
							}
						}
						else  //sets a melee weapon
						{
							if (it->hasTag("Melee"))
							{
								inventComp->setEquipedItemID(it->getID());
								std::cout << "AI weapon equiped " << it->getName() << std::endl;
								break;
							}
						}
					}
				}
		}
	}*/
}

LuaEntityHandle LuaEntityHandle::getEnemy()
{
	/*if (_e->hasComponent(std::type_index(typeid(AIComponent))))
	{
		return LuaEntityHandle(_e->get<AIComponent>()->_enemy.get());
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a AI component" << std::endl;
		return nullptr;
	}*/
	return LuaEntityHandle(_entityManager, 0);
}

bool LuaEntityHandle::hasEnemy()
{
	/*if (_e->hasComponent(std::type_index(typeid(AIComponent))))
	{
		if (_e->get<AIComponent>()->_enemy != nullptr)
			if (_e->get<AIComponent>()->_enemy->get<HealthComponent>()->_alive)
				return true;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a AI component" << std::endl;
	}*/
	return false;
}

void LuaEntityHandle::wander()
{
	/*auto aiComp = _e->get<AIComponent>();

	//if (_e->get<CollisionComponent>()->collisionContainsPoint(aiComp->_targetPosition))
	if (this->getDistanceToPosition(aiComp->_targetPosition) < 10)
	{
		this->randomizeTargetPosition();
	}

	this->moveTowardsPosition(aiComp->_targetPosition);*/
}

void LuaEntityHandle::followRoute()
{
	AIComponent* aiComp = _entityManager->getComponent<AIComponent>(_e, ComponentType::AI);
	CollisionComponent* colComp = _entityManager->getComponent<CollisionComponent>(_e, ComponentType::Collision);
	MovementComponent* movComp = _entityManager->getComponent<MovementComponent>(_e, ComponentType::Movement);

	if (aiComp->_currentRouteNode < aiComp->_routeNodes.size()) {
		if (this->getDistanceBetweenPositions(aiComp->_routeNodes[aiComp->_currentRouteNode], sf::Vector2f(colComp->_colRect.left + colComp->_colRect.width / 2, colComp->_colRect.top + colComp->_colRect.height / 2)) < 10)
		{
			aiComp->_currentRouteNode++;
			//if (aiComp->_routeNodes.size() == aiComp->_currentRouteNode)	//if reached last point
				//movComp->_movement = sf::Vector2f(0, 0);
		}

		if (aiComp->_routeNodes.size() != aiComp->_currentRouteNode) {	//if not reached last point
			this->moveTowardsPosition(aiComp->_routeNodes[aiComp->_currentRouteNode]);
		}
	} else {
		//if last node reached
		std::cout << "reached destination" << std::endl;
		_entityManager->getComponent<ScheduleComponent>(_e, ComponentType::Schedule)->advanceCurrentTask();
	}
}

void LuaEntityHandle::flee()
{
	/*if (_e->hasComponent(std::type_index(typeid(MovementComponent))) && _e->hasComponent(std::type_index(typeid(PositionComponent))) && _e->get<AIComponent>()->_enemy->hasComponent(std::type_index(typeid(PositionComponent))))
	{
		auto AIAIComp = _e->get<AIComponent>();

		auto AImoveComp = _e->get<MovementComponent>();
		auto AIPosComp = _e->get<PositionComponent>();
		auto enemyPosComp = AIAIComp->_enemy->get<PositionComponent>();

		AImoveComp->_movement = AIPosComp->_position - enemyPosComp->_position;
	}
	else
	{
		std::cout << "Entity does not have Movement Component" << std::endl;
	}*/
}

void LuaEntityHandle::randomizeTargetPosition()
{
	/*auto aiComp = _e->get<AIComponent>();

	unsigned int xPos = rand() % 500;
	unsigned int yPos = rand() % 500;

	aiComp->_targetPosition = sf::Vector2f(xPos, yPos);*/
}

void LuaEntityHandle::stopMotion()
{
	/*if (_e->hasComponent(std::type_index(typeid(MovementComponent))))
	{
		_e->get<MovementComponent>()->_movement = sf::Vector2f(0.f, 0.f);
	}
	else
	{
		std::cout << "Entity doesn't have movement component" << std::endl;
	}*/
}

void LuaEntityHandle::attackEnemy()
{
	/*auto invent = _e->get<InventoryComponent>();
	Entity* equipedItem = invent->getEquipedItem();

	if (equipedItem->hasTag("Weapon"))
	{
		luabridge::LuaRef table = luabridge::getGlobal(GameManager::L, equipedItem->getScriptTable().c_str());
		luabridge::LuaRef ItemTable = table["ItemComponent"];
		luabridge::LuaRef activateFunc = ItemTable["activate"];			

		if (activateFunc.isFunction())
		{
			activateFunc(LuaEntityHandle(_e), LuaEntityHandle(equipedItem));
		}
	}*/
}


bool LuaEntityHandle::timerTicked(std::string timerName)
{
	/*if (_e->hasComponent(std::type_index(typeid(TimerComponent))))
	{
		auto& timer = _e->get<TimerComponent>()->_timers.find(timerName);
		if (timer != _e->get<TimerComponent>()->_timers.end())
		{
			luabridge::LuaRef table = luabridge::getGlobal(GameManager::L, _e->getScriptTable().c_str());
			luabridge::LuaRef TimerTable = table["TimerComponent"];
			luabridge::LuaRef actualTimerTable = TimerTable[timerName];
			unsigned int tickTime = actualTimerTable["tickTime"].cast<unsigned int>();

			if (timer->second->getElapsedTime().asMilliseconds() > tickTime)
			{
				timer->second->restart();	//temp needs own function
				return true;
			}
		}
	}
	else
	{
		std::cout << "LuaEntityHandle_ doesn't have a Timer component" << std::endl;
	}*/

	return false;
}

void LuaEntityHandle::notifyObservers(const std::string& eventType)
{
	_entityManager->getLuaManager()->notifyObservers(_e, eventType);
}

sf::Vector2f LuaEntityHandle::getTargetPosition() const
{
	if (_entityManager->getComponent<AIComponent>(_e, ComponentType::AI))
	{
		return _entityManager->getComponent<AIComponent>(_e, ComponentType::AI)->_targetPosition;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a AI component" << std::endl;
	}

	return sf::Vector2f(0, 0);
}

void LuaEntityHandle::setTargetPosition(sf::Vector2f newTargetPos)
{
	if (_entityManager->getComponent<AIComponent>(_e, ComponentType::AI))
	{
		_entityManager->getComponent<AIComponent>(_e, ComponentType::AI)->_targetPosition = newTargetPos;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a AI component" << std::endl;
	}
}

sf::Vector2f LuaEntityHandle::getGuardPosition() const
{
	/*if (_e->hasComponent(std::type_index(typeid(AIComponent))))
	{
		return _e->get<AIComponent>()->_guardPosition;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a AI component" << std::endl;
		return sf::Vector2f(0, 0);
	}*/
	return sf::Vector2f(0, 0);
}

void LuaEntityHandle::setGuardPosition(sf::Vector2f newGuardPos)
{
	/*if (_e->hasComponent(std::type_index(typeid(AIComponent))))
	{
		_e->get<AIComponent>()->_guardPosition = newGuardPos;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a AI component" << std::endl;
	}*/
}

sf::Vector2f LuaEntityHandle::getPosition() const
{
	if (_entityManager->hasComponent(_e, ComponentType::Position))
	{
		PositionComponent* posComp = _entityManager->getComponent<PositionComponent>(_e, ComponentType::Position);
		return posComp->_position;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a Position component" << std::endl;
	}
	return sf::Vector2f(0, 0);
}

void LuaEntityHandle::setPosition(sf::Vector2f pos)
{
	if (_entityManager->hasComponent(_e, ComponentType::Position))
	{
		PositionComponent* posComp = _entityManager->getComponent<PositionComponent>(_e, ComponentType::Position);
		posComp->_position = pos;
		std::cout << "Entity position set" << pos.x << " " << pos.y << std::endl;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a Position component" << std::endl;
	}
}
