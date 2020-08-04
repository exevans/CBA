#include "Entity.h"
#include "Component.h"
#include "LuaEntityHandle.h"
#include <iostream>

unsigned int Entity::_nextID = 0;

Entity::Entity()
	:_ID(_nextID)
	//_luaEntHandle(this)
{
	_nextID++;
}

Entity::~Entity()
{

}

bool Entity::hasTag(std::string checkTag)
{
	for (std::vector<std::string>::iterator it = _tags.begin(); it != _tags.end(); ++it)
	{
		if (*it == checkTag)
			return true;
	}

	return false;
}

/*void Entity::loadScript(luabridge::lua_State* L, const std::string& scriptFilename, const std::string& tableName)
{
	/*_scriptPath = scriptFilename;
	_scriptTable = tableName;

	{
		luabridge::LuaRef table = luabridge::getGlobal(L, tableName.c_str());
		if (table.isTable())
		{
			_name = table["name"].cast<std::string>();
			std::cout << "New item: " << _name << std::endl;

			if (table["entTypeID"].isString())	//entity type ID
				_entityTypeID = table["entTypeID"].cast<std::string>();
			else
				_entityTypeID = " ";

			if (table["Tags"].isTable())
			{
				luabridge::LuaRef tagsTable = table["Tags"];

				for (int i = 0; i < tagsTable.length(); ++i)
				{
					luabridge::LuaRef elementRef = tagsTable[i + 1];
					//add tag to tags vector
					_tags.push_back(elementRef.cast<std::string>());
					//std::cout << "New tag added: " << _tags[_tags.size() - 1] << std::endl;
				}
			}

			//ADD THE COMPONENTS
			if (table["HealthComponent"].isTable())
			{
				unsigned int health = 0;

				luabridge::LuaRef compTable = table["HealthComponent"];
				if (compTable["health"].isNumber())
				{
					health = compTable["health"].cast<int>();
				}

				this->addComponent(std::shared_ptr<Component>(new HealthComponent(health)));
			}
			if (table["PositionComponent"].isTable())
			{
				this->addComponent(std::shared_ptr<Component>(new PositionComponent()));
			}
			if (table["MovementComponent"].isTable())
			{
				luabridge::LuaRef compTable = table["MovementComponent"];

				float speed = compTable["speed"].cast<float>();

				this->addComponent(std::shared_ptr<Component>(new MovementComponent(speed)));
			}
			if (table["TextureComponent"].isTable())
			{
				luabridge::LuaRef compTable = table["TextureComponent"];

				std::string filePath = compTable["textureFilepath"].cast<std::string>();

				sf::IntRect texRect;

				if (compTable["frame"].isTable())	//no animations
				{
					luabridge::LuaRef frameTable = compTable["frame"];

					texRect.left = frameTable[1].cast<unsigned int>();
					texRect.top = frameTable[2].cast<unsigned int>();

					texRect.width = frameTable[3].cast<unsigned int>();
					texRect.height = frameTable[4].cast<unsigned int>();
				}
				else if (compTable["animated"].cast<bool>() == true && compTable["animations"].isTable() && compTable["defaultAnimation"].isString())	//animated
				{
					luabridge::LuaRef animationsTable = compTable["animations"];
					luabridge::LuaRef animationTable = animationsTable[compTable["defaultAnimation"].cast<std::string>()];

					if (animationTable["frame"].isTable())	//single frame for animation
					{
						luabridge::LuaRef frameTable = animationTable["frame"];

						texRect.left = frameTable[1].cast<unsigned int>();
						texRect.top = frameTable[2].cast<unsigned int>();

						texRect.width = frameTable[3].cast<unsigned int>();
						texRect.height = frameTable[4].cast<unsigned int>();
					}
					else if (animationTable["startFrame"].isTable() && animationTable["endFrame"].isTable())	//has start/end frame for animation
					{
						luabridge::LuaRef startFrameTable = animationTable["startFrame"];

						texRect.left = startFrameTable[1].cast<unsigned int>();
						texRect.top = startFrameTable[2].cast<unsigned int>();

						texRect.width = startFrameTable[3].cast<unsigned int>();
						texRect.height = startFrameTable[4].cast<unsigned int>();
					}
				}

				unsigned int zOrder = compTable["zOrder"].cast<unsigned int>();

				this->addComponent(std::shared_ptr<Component>(new TextureComponent(filePath, texRect, get<PositionComponent>(), zOrder)));
			}
			if (table["CollisionComponent"].isTable())
			{
				luabridge::LuaRef compTable = table["CollisionComponent"];

				std::string collisionType = compTable["collisionType"].cast<std::string>();

				if (!compTable["colRect"].isTable() && this->hasComponent(std::type_index(typeid(TextureComponent))))
					this->addComponent(std::shared_ptr<Component>(new CollisionComponent(this->get<TextureComponent>())));
				else
				{
					if (compTable["colRect"].isTable())
					{
						luabridge::LuaRef colRectTable = compTable["colRect"];

						sf::Vector2f colRectSize;
						sf::Vector2f localColRectPos;

						localColRectPos.x = colRectTable[1].cast<float>();
						localColRectPos.y = colRectTable[2].cast<float>();

						colRectSize.x = colRectTable[3].cast<float>();
						colRectSize.y = colRectTable[4].cast<float>();

						this->addComponent(std::shared_ptr<Component>(new CollisionComponent(this->get<PositionComponent>()->_position, localColRectPos, colRectSize)));
					}
				}
			}
			if (table["InteractComponent"].isTable())
			{
				luabridge::LuaRef compTable = table["InteractComponent"];
				if (compTable["interactRect"].isTable())
				{
					luabridge::LuaRef interactRectTable = compTable["interactRect"];
					sf::Vector2f colRectSize;
					sf::Vector2f localColRectPos;

					localColRectPos.x = interactRectTable[1].cast<float>();
					localColRectPos.y = interactRectTable[2].cast<float>();

					colRectSize.x = interactRectTable[3].cast<float>();
					colRectSize.y = interactRectTable[4].cast<float>();

					this->addComponent(std::shared_ptr<Component>(new InteractComponent(this->get<PositionComponent>()->_position, localColRectPos, colRectSize)));
				}
			}
			if (table["InventoryComponent"].isTable())
			{
				this->addComponent(std::shared_ptr<Component>(new InventoryComponent()));	//creates main component

				luabridge::LuaRef compTable = table["InventoryComponent"];
				luabridge::LuaRef inventItemTable = compTable["Items"];

				if (inventItemTable.isTable())
				{
					for (int i = 0; i < inventItemTable.length(); ++i)
					{
						luabridge::LuaRef elementRef = inventItemTable[i + 1];

						std::shared_ptr<Entity> inventItem(new Entity);
						inventItem->loadScript(L, "Assets/Scripts/Items.lua", elementRef.cast<std::string>());

						this->get<InventoryComponent>()->addEntity(std::move(inventItem));	//adds item to component
					}
				}
			}
			if (table["InputComponent"].isTable())
			{
				this->addComponent(std::shared_ptr<Component>(new InputComponent()));
			}
			if (table["AnimationComponent"].isTable())
			{
				luabridge::LuaRef compTable = table["AnimationComponent"];

				this->addComponent(std::shared_ptr<Component>(new AnimationComponent(this->get<TextureComponent>(), compTable["textureRate"].cast<float>())));

				//new stuff
				compTable = table["TextureComponent"];
				if (compTable["animated"].cast<bool>() == true && compTable["animations"].isTable() && compTable["defaultAnimation"].isString())	//animated
				{
					luabridge::LuaRef animationsTable = compTable["animations"];
					luabridge::LuaRef animationTable = animationsTable[compTable["defaultAnimation"].cast<std::string>()];

					if (animationTable["startFrame"].isTable() && animationTable["endFrame"].isTable())	//has start/end frame for animation
					{
						luabridge::LuaRef startFrameTable = animationTable["startFrame"];	//start frame

						sf::Vector2u startFramePos;
						startFramePos.x = startFrameTable[1].cast<unsigned int>();
						startFramePos.y = startFrameTable[2].cast<unsigned int>();

						this->get<AnimationComponent>()->_startFramePos = startFramePos;


						luabridge::LuaRef endFrameTable = animationTable["endFrame"];	//end frame

						sf::Vector2u endFramePos;
						endFramePos.x = endFrameTable[1].cast<unsigned int>();
						endFramePos.y = endFrameTable[2].cast<unsigned int>();

						this->get<AnimationComponent>()->_endFramePos = endFramePos;
					}
				}

			}
			if (table["ItemComponent"].isTable())
			{
				luabridge::LuaRef compTable = table["ItemComponent"];

				this->addComponent(std::shared_ptr<Component>(new ItemComponent()));
			}
			if (table["SpawnerComponent"].isTable())
			{
				luabridge::LuaRef compTable = table["SpawnerComponent"];

				this->addComponent(std::shared_ptr<Component>(new SpawnerComponent()));
			}
			if (table["AIComponent"].isTable())
			{
				luabridge::LuaRef compTable = table["AIComponent"];
				std::string AIType = compTable["npcType"].cast<std::string>();

				this->addComponent(std::shared_ptr<Component>(new AIComponent(AIType)));

				if (compTable["PatrolPositions"].isTable())	
				{
					luabridge::LuaRef patrolTable = compTable["PatrolPositions"];*/

					/*for (auto& patrolData : getKeyValueMap(table))
					{
						luabridge::LuaRef positionTable = patrolData.second;
						std::cout << "TESTING" << patrolData.first << std::endl;
						sf::Vector2f newPatrolPos = sf::Vector2f(positionTable[1].cast<float>(), positionTable[2].cast<float>());
						this->get<AIComponent>()->_patrolPositions.push_back(newPatrolPos);
					}*/

					/*if (patrolTable["position1"].isTable())
					{
						luabridge::LuaRef positionTable = patrolTable["position1"];
						sf::Vector2f newPatrolPos = sf::Vector2f(positionTable[1].cast<float>(), positionTable[2].cast<float>());
						this->get<AIComponent>()->_patrolPositions.push_back(newPatrolPos);

						if (patrolTable["position2"].isTable())
						{
							luabridge::LuaRef positionTable = patrolTable["position2"];
							newPatrolPos = sf::Vector2f(positionTable[1].cast<float>(), positionTable[2].cast<float>());
							this->get<AIComponent>()->_patrolPositions.push_back(newPatrolPos);

							if (patrolTable["position3"].isTable())
							{
								luabridge::LuaRef positionTable = patrolTable["position3"];
								newPatrolPos = sf::Vector2f(positionTable[1].cast<float>(), positionTable[2].cast<float>());
								this->get<AIComponent>()->_patrolPositions.push_back(newPatrolPos);

								if (patrolTable["position4"].isTable())
								{
									luabridge::LuaRef positionTable = patrolTable["position4"];
									newPatrolPos = sf::Vector2f(positionTable[1].cast<float>(), positionTable[2].cast<float>());
									this->get<AIComponent>()->_patrolPositions.push_back(newPatrolPos);
								}
							}
						}
					}
				}
			}
			if (table["DialogueComponent"].isTable())
			{
				luabridge::LuaRef compTable = table["DialogueComponent"];
				unsigned int conversationID = compTable["conversationID"].cast<unsigned int>();

				this->addComponent(std::shared_ptr<Component>(new DialogueComponent(conversationID)));
			}
			if (table["QuestComponent"].isTable())
			{
				luabridge::LuaRef compTable = table["QuestComponent"];
				std::string questDataPath = compTable["questDataPath"].cast<std::string>();

				this->addComponent(std::shared_ptr<Component>(new QuestComponent(questDataPath)));
			}
			if (table["ScriptStateMachineComponent"].isTable())
			{
				luabridge::LuaRef compTable = table["ScriptStateMachineComponent"];

				this->addComponent(std::shared_ptr<Component>(new ScriptStateMachineComponent()));
			}
			if (table["TimerComponent"].isTable())
			{
				this->addComponent(std::shared_ptr<Component>(new TimerComponent()));

				luabridge::LuaRef compTable = table["TimerComponent"];
				if (compTable["cooldownTimer"].isTable())
				{
					std::unique_ptr<sf::Clock> timer(new sf::Clock());
					this->get<TimerComponent>()->_timers["cooldownTimer"] = std::move(timer);
				}
				if (compTable["killTimer"].isTable())
				{
					std::unique_ptr<sf::Clock> timer(new sf::Clock());
					this->get<TimerComponent>()->_timers["killTimer"] = std::move(timer);
				}
				if (compTable["weaponTimer"].isTable())
				{
					std::unique_ptr<sf::Clock> timer(new sf::Clock());
					this->get<TimerComponent>()->_timers["weaponTimer"] = std::move(timer);
				}
			}

			// call init function
			std::unique_ptr<luabridge::LuaRef> initFunc = std::make_unique<luabridge::LuaRef>(table["init"]);
			if (initFunc->isFunction())
				(*initFunc)(LuaEntityHandle(this));
		}
	}
}*/

void Entity::addComponent(std::shared_ptr<Component> newComponent)
{
	newComponent->setOwnerID(_ID);
	newComponent->setOwnerEntity(shared_from_this());

	_componentsKey += newComponent->getComponentID();

	_components[std::type_index(typeid(*newComponent))] = std::move(newComponent);
}

void Entity::removeComponents()
{
	//for (std::map<std::type_index, std::shared_ptr<Component>>::iterator it = _components.begin(); it != _components.end(); ++it)
		//it->second->setInactive();
}

bool Entity::hasComponent(std::type_index type) const
{
	auto& it = _components.find(type);
	if (it != _components.end())
		return true;

	return false;
}

/*LuaEntityHandle Entity::getLuaEntityHandle() const
{
	//return _luaEntHandle;
}*/

void Entity::setParent(std::shared_ptr<Entity> parent)
{
	_parent = parent;
	_child = true;
}

bool Entity::hasParent() const
{
	return _child;
}

Entity* Entity::getParent() const
{
	return _parent.get();
}

unsigned int Entity::getComponentNum() const
{
	return _components.size();
}

const unsigned int Entity::getID() const
{
	return _ID;
}

const std::string& Entity::getTypeID() 
{
	return _entityTypeID;
}

const std::string & Entity::getName()
{
	return _name;
}

const std::string& Entity::getScriptFilePath()
{
	return _scriptPath;
}

const std::string& Entity::getScriptTable()
{
	return _scriptTable;
}

const _Uint32t Entity::getComponentKey() const
{
	return _componentsKey;
}

const bool Entity::isActive() const
{
	if (_entityState != Entitys::Destroyed)
		return true;

	return false;
}

void Entity::setInactive()
{
	_entityState = Entitys::Destroyed;

	//for (std::map<std::type_index, std::shared_ptr<Component>>::iterator it = _components.begin(); it != _components.end(); ++it)
		//it->second->setInactive();
}

void Entity::setFileID(unsigned int fileID)
{
	_fileID = fileID;
}

const unsigned int Entity::getFileID() const
{
	return _fileID;
}

void Entity::onNotify(Entity* otherEnt, std::string eventType)
{	
	if (eventType == "EVENT_ENTITY_REMOVED")
	{
		/*if (this->hasComponent(std::type_index(typeid(AIComponent))))
		{
			auto aiComp = this->get<AIComponent>();
			if (aiComp->_enemy != nullptr && aiComp->_enemy->getID() == otherEnt->getID())
			{
				aiComp->_enemy = nullptr; //set to not an enemy any more
			}
		}*/

		/*if (this->hasComponent(std::type_index(typeid(CollisionComponent))))
		{
			auto colComp = this->get<CollisionComponent>();
			if (colComp->collidedEntityID == otherEnt->getID())
			{
				luabridge::LuaRef HPTable = luabridge::getGlobal(GameManager::L, this->getScriptTable().c_str());
				luabridge::LuaRef ColTable = HPTable["CollisionComponent"];

				this->get<CollisionComponent>()->collidedEntityID = -1;
				luabridge::LuaRef collideExit = ColTable["OnExit"];
				if (collideExit.isFunction())
					collideExit(otherEnt->getLuaEntityHandle(), this->getLuaEntityHandle());
			}
		}*/
	}
	//sends notify request into lua logic
	/*luabridge::LuaRef table = luabridge::getGlobal(GameManager::L, this->getScriptTable().c_str());
	luabridge::LuaRef observerTable = table["ObserverComponent"];

	if (observerTable.isTable())	//if observerComponent exists
	{
		luabridge::LuaRef onNotifyFunc = observerTable["onNotify"];

		if (onNotifyFunc.isFunction())
		{
			//onNotifyFunc(LuaEntityHandle(this), LuaEntityHandle(otherEnt), eventType);
		}
	}*/
}
