#include "Component.h"
#include "Entity.h"
#include "ShaderManager.h"
#include "SpriteSheet.h"
#include "EntityManager.h"
#include "TextureManager.h"
#include "LightManager.h"
#include "Direction.h"
#include <iostream>

Component::Component(const ComponentType& componentType) : _compType(componentType)
{
	//std::bitset<32> componentId(componentCode);
	//this->_componentID = componentId.to_ulong();
}

void Component::setOwnerID(unsigned int ownID)
{
	_entityID = ownID;
}

void Component::setOwnerEntity(std::shared_ptr<Entity> ownerEntity)
{
	_ownerEntity = ownerEntity;
}

unsigned int Component::getOwnerID() const
{
	return _entityID;
}

Entity* Component::getOwnerEntity() const
{
	return std::shared_ptr<Entity>(_ownerEntity).get();
}

const ComponentType Component::getComponentType() {
	return _compType;
}

void Component::setActive(bool newState) 
{
	_active = newState;
}

bool Component::isActive() const
{
	return _active;
}

Component::~Component()
{

}

const _Uint32t Component::getComponentID() const
{
	return _componentID;
}


HealthComponent::HealthComponent(/*unsigned int startingHp*/)
	:Component(ComponentType::Health), _health(0), _alive(true)
{
	
}

void HealthComponent::readIn(const sol::table& compTable) {
	sol::object hlth = compTable["health"];
	if (hlth.is<int>()) {
		_health = hlth.as<int>();
	}
}


PositionComponent::PositionComponent()
	:Component(ComponentType::Position),
	_position(0, 0)
{

}

void PositionComponent::readIn(const sol::table& compTable) {
	
}

void PositionComponent::moveBy(const sf::Vector2f movement) {
	_position += (movement);

	// if position is out of range
	if (_position.x < 0)
	{
		_position.x = 0;
	}
	if (_position.y < 0)
	{
		_position.y = 0;
	}
}

MovementComponent::MovementComponent(/*float speed*/)
	:Component(ComponentType::Movement)
	, _maxVelocity(0)
	, _experiencesFriction(false)
{
	_speed = sf::Vector2f(100, 100);
	_velocity = sf::Vector2f(0.f, 0.f);
	_prevVelocity = sf::Vector2f(0.f, 0.f);
	_acceleration = sf::Vector2f(0.f, 0.f);
}

void MovementComponent::readIn(const sol::table& compTable) {
	sol::object oSpeed = compTable["speed"];
	float speed = oSpeed.as<float>();
	_maxVelocity = speed;
	_speed = sf::Vector2f(speed, speed);
	//_speed = sf::Vector2f(speed, speed);
	sol::object oFriction = compTable["friction"];
	if (oFriction.valid())
		_experiencesFriction = oFriction.as<bool>();
}

void MovementComponent::accelerate(const sf::Vector2f& vec) {
	_acceleration += vec;
}

void MovementComponent::addVelocity(const sf::Vector2f& vel) {
	_velocity += vel;
	if (std::abs(_velocity.x) > _maxVelocity) {
		_velocity.x = _maxVelocity * (_velocity.x / std::abs(_velocity.x));
	}
	if (std::abs(_velocity.y) > _maxVelocity) {
		_velocity.y = _maxVelocity * (_velocity.y / std::abs(_velocity.y));
	}
}

void MovementComponent::setVelocity(sf::Vector2f vel) {
	_velocity = vel;
}

void MovementComponent::applyFriction(const sf::Vector2f& vel) {
	if (_velocity.x != 0 && vel.x != 0) {
		if (std::abs(_velocity.x) - std::abs(vel.x) < 0) {
			_velocity.x = 0;
		}
		else {
			_velocity.x += (_velocity.x > 0 ? std::abs(vel.x) * -1 : std::abs(vel.x));
		}
	}

	if (_velocity.y != 0 && vel.y != 0) {
		if (std::abs(_velocity.y) - std::abs(vel.y) < 0) {
			_velocity.y = 0;
		}
		else {
			_velocity.y += (_velocity.y > 0 ? std::abs(vel.y) * -1 : std::abs(vel.y));
		}
	}
}

void MovementComponent::move(const Direction& dir) {
	if (dir == Direction::Up) 
		_acceleration.y -= _speed.y;
	else if (dir == Direction::Down)
		_acceleration.y += _speed.y;
	else if (dir == Direction::Left)
		_acceleration.x -= _speed.x;
	else if (dir == Direction::Right) 
		_acceleration.x += _speed.x;
}

void MovementComponent::setDirection(const Direction& dir) {
	_direction = dir;
}


TextureComponent::TextureComponent(/*const std::string& filename, sf::IntRect texRect, PositionComponent* posComp, unsigned int z_Order*/)
	:Component(ComponentType::Texture),
	_zOrder(0),
	_texturePath("")
{

}

void TextureComponent::readIn(const sol::table& compTable) {
	std::string filePath = compTable["textureFilepath"];
	sf::IntRect texRect;

	if (compTable["frame"] != sol::nil)	//no animations
	{
		sol::table frameTable = compTable["frame"];

		texRect.left = frameTable[1];
		texRect.top = frameTable[2];

		texRect.width = frameTable[3];
		texRect.height = frameTable[4];
	}
	else if (compTable["animated"] == true && compTable["animations"] != sol::nil && compTable["defaultAnimation"] != sol::nil)	//animated
	{
		sol::table animationsTable = compTable["animations"];
		sol::table animationTable = animationsTable[compTable["defaultAnimation"]];

		if (animationTable["frame"]!= sol::nil)	//single frame for animation
		{
			sol::table frameTable = animationTable["frame"];

			texRect.left = frameTable[1];
			texRect.top = frameTable[2];

			texRect.width = frameTable[3];
			texRect.height = frameTable[4];
		}
		/*else if (animationTable["startFrame"].isNumber() && animationTable["endFrame"].isNumber())	//has start/end frame for animation
		{
			luabridge::LuaRef spriteSizeTable = animationTable["spriteSize"];

			//texRect.left = startFrameTable[1].cast<unsigned int>();
			//texRect.top = startFrameTable[2].cast<unsigned int>();

			spriteSize.x = spriteSizeTable[1].cast<unsigned int>();
			spriteSize.y = spriteSizeTable[2].cast<unsigned int>();
		}*/
	}

	unsigned int zOrder = compTable["zOrder"];

	_texturePath = filePath;
	_zOrder = zOrder;

	//_textureSize = spriteSize;

	TextureManager::addTexture(_texturePath);
	//temp shader test
	//std::string sName = "Pixelate";
	//std::string sVert = "Assets/Shaders/Pixelate.vert";
	//std::string sFrag = "Assets/Shaders/Pixelate.frag";
	//ShaderManager::addShader(sName, sVert, sFrag, Shader::Both);

	_sprite.setTexture(TextureManager::getTextureRef(_texturePath));
	_sprite.setTextureRect(texRect);

	//VERY TEMP
	if (compTable["scale"] != sol::nil) {
		_sprite.setScale(compTable["scale"], compTable["scale"]);
	}
	else {
		_sprite.setScale(3, 3);
	}

	_sprite.setOrigin(_sprite.getPosition().x + _sprite.getGlobalBounds().width / 2, _sprite.getPosition().y + _sprite.getGlobalBounds().height);	//set origin to centre of texture
	_sprite.setOrigin(_sprite.getOrigin().x / _sprite.getScale().x, _sprite.getOrigin().y/_sprite.getScale().y);
}


SpriteSheetComponent::SpriteSheetComponent() : Component(ComponentType::SpriteSheet), _spriteSheet(nullptr) {

}

void SpriteSheetComponent::readIn(const sol::table& compTable) {
	
}

void SpriteSheetComponent::create(TextureManager* texManager, std::string& sheetName, const sol::table& compTable) {
	_sheetName = sheetName;
	_spriteSheet = new SpriteSheet(texManager);
	//_spriteSheet->loadSheet("Assets/Textures/SpriteSheets" + _sheetName + ".png", compTable);
}

InputComponent::InputComponent()
	:Component(ComponentType::Input),
	_controllable(true)
{
	
}

void InputComponent::readIn(const sol::table& compTable) {

}


CollisionComponent::CollisionComponent()
	:Component(ComponentType::Collision)
{

}


void CollisionComponent::readIn(const sol::table& compTable, PositionComponent* posComp, TextureComponent* texComp) {
	//std::string collisionType = compTable["collisionType"];

	if (compTable["colRect"] == sol::nil && texComp != nullptr) {
		_localColRectOffset = sf::Vector2f(0.f, 0.f);
		_colRect.left = texComp->_sprite.getPosition().x;
		_colRect.top = texComp->_sprite.getPosition().y;
		_colRect.width = texComp->_sprite.getGlobalBounds().width;
		_colRect.height = texComp->_sprite.getGlobalBounds().height;
	}
	else if (compTable["colRect"] != sol::nil && posComp != nullptr) {
		sol::table colRectTable = compTable["colRect"];

		sf::Vector2f colRectSize;
		sf::Vector2f localColRectPos;

		localColRectPos.x = colRectTable[1];
		localColRectPos.y = colRectTable[2];
		colRectSize.x = colRectTable[3];
		colRectSize.y = colRectTable[4];

		_colRect.left = posComp->_position.x + localColRectPos.x;
		_colRect.top = posComp->_position.y + localColRectPos.y;
		_colRect.width = colRectSize.x;
		_colRect.height = colRectSize.y;

		_localColRectOffset = localColRectPos;
	}

	//scale if necessary
	if (texComp != nullptr) {
		_colRect.width *= texComp->_sprite.getScale().x;
		_colRect.height *= texComp->_sprite.getScale().y;
	}
}

bool CollisionComponent::collisionContainsPoint(sf::Vector2f pointPos)
{
	if (_colRect.contains(pointPos))
		return true;

	return false;
}


AnimationComponent::AnimationComponent()
	:Component(ComponentType::Animation)
	/*_animationRate(0),
	_elapsedTime(0),
	_loop(false),
	_playing(false)*/
{
	
}

void AnimationComponent::readIn(const sol::table& compTable) {
	if (compTable["spriteSheet"] != sol::nil) {
		_spriteSheet.loadSheet(compTable["spriteSheet"]);
	}
	//_currentTexPos = sf::Vector2u(texComp->_sprite.getTextureRect().left, texComp->_sprite.getTextureRect().top);
	//_imageSize = TextureManager::getTextureRef(texComp->_texturePath).getSize();
	//this->addComponent(std::shared_ptr<Component>(new AnimationComponent(this->get<TextureComponent>(), compTable["textureRate"].cast<float>())));

	//new stuff
	/*if (compTable["animated"] == true && compTable["animations"] != sol::nil && compTable["defaultAnimation"] != sol::nil)	//animated
	{
		sol::table animationsTable = compTable["animations"];
		sol::table animationTable = animationsTable[compTable["defaultAnimation"]];

		/*if (animationTable["spriteSize"].isTable())	//has start/end frame for animation
		{
			_spriteSize.x = animationTable["spriteSize"][1].cast<unsigned int>();
			_spriteSize.y = animationTable["spriteSize"][2].cast<unsigned int>();

			_startRow = animationTable["startRow"].cast<unsigned int>();
			_startFrame = animationTable["startFrame"].cast<unsigned int>();
			_endFrame = animationTable["endFrame"].cast<unsigned int>();

			_currentFrame = _startFrame;

			/*luabridge::LuaRef startFrameTable = animationTable["startFrame"];	//start frame

			sf::Vector2u startFramePos;
			startFramePos.x = startFrameTable[1].cast<unsigned int>();
			startFramePos.y = startFrameTable[2].cast<unsigned int>();

			_startFramePos = startFramePos;


			luabridge::LuaRef endFrameTable = animationTable["endFrame"];	//end frame

			sf::Vector2u endFramePos;
			endFramePos.x = endFrameTable[1].cast<unsigned int>();
			endFramePos.y = endFrameTable[2].cast<unsigned int>();

			_endFramePos = endFramePos;
		}
		playAnimation(animationTable , true);
	}*/
}

/*void AnimationComponent::stopCurrentAnimation() {
	_playing = false;
}

void AnimationComponent::playAnimation(const sol::table& animationTable, const bool loop) {
	if (animationTable["spriteSize"] != sol::nil)	//has start/end frame for animation
	{
		sol::table spriteSizeTable = animationTable["spriteSize"];
		unsigned int test = spriteSizeTable[1];
		_spriteSize.x = spriteSizeTable[1];
		_spriteSize.y = spriteSizeTable[2];

		_startRow = animationTable["startRow"];
		_startFrame = animationTable["startFrame"];
		_endFrame = animationTable["endFrame"];

		_currentFrame = _startFrame;
	}

	_playing = true;
	_loop = loop;
}*/


ItemComponent::ItemComponent()
	:Component(ComponentType::Item)
{
	
}

void ItemComponent::readIn(const sol::table& compTable) {

}


InventoryComponent::InventoryComponent()
	:Component(ComponentType::Inventory)
	//_maxItemNumber(6)
{
	
}

void InventoryComponent::readIn(const sol::table& compTable) {
	sol::table inventItemTable = compTable["Items"];

	if (inventItemTable != sol::nil) {
		for (int i = 0; i < inventItemTable.size(); ++i) {
			std::string elementRef = inventItemTable[i + 1];
		}
	}
}


/*void InventoryComponent::addEntity(std::shared_ptr<Entity> newItem)
{
	if (newItem->hasComponent(std::type_index(typeid(ItemComponent))))
	{
		std::cout << "INVENTORY GROWING " << std::endl;

		if (_items.size() == 0)
			_equipedItemID = newItem->getID();

		_items.push_back(newItem);		

		std::cout << "Entity added to inventory" << std::endl;
	}
}

void InventoryComponent::removeEntity(unsigned int id)
{
	for (unsigned int i = 0; i < _items.size(); i++)
		if (_items[i]->getID() == id)
		{
			_items.erase(_items.begin() + i);
			std::cout << "Entity removed from inventory" << std::endl;
			break;
		}

	this->setEquipedItem(0);
}

void InventoryComponent::removeEntity(std::string typeID)
{
	for (unsigned int i = 0; i < _items.size(); i++)
		if (_items[i]->getTypeID() == typeID)
		{
			_items.erase(_items.begin() + i);
			std::cout << "Entity removed from inventory" << std::endl;
			break;
		}

	this->setEquipedItem(0);
}

void InventoryComponent::clearInventory()
{
	//_items.clear();
}

Entity* InventoryComponent::getItem(unsigned int id)
{
	for (auto& it : _items)
		if (it->getID() == id)
			return it.get();

	return nullptr;
}

Entity* InventoryComponent::getEquipedItem()
{
	for (auto& it : _items)
		if (it->getID() == _equipedItemID)
			return it.get();

	return nullptr;
}

void InventoryComponent::setEquipedItem(unsigned int inventIndex)
{
	if (_items.size() > inventIndex)
	{
		_equipedItemID = _items[inventIndex]->getID();
		std::cout << "Item Equiped: " << _items[inventIndex]->getName() << std::endl;
	}
}

void InventoryComponent::setEquipedItemID(unsigned int itemId)
{
	for (auto& it : _items)
	{
		if (it->getID() == itemId)
			_equipedItemID = it->getID();
		std::cout << "Item Equiped: " << it->getName() << std::endl;
	}
}

unsigned int InventoryComponent::getEquipedItemVectVal()
{
	if (_items.size() > 0)
	{
		for (size_t i = 0; i < _items.size(); i++)
		{
			if (_items[i]->getID() == _equipedItemID)
				return i;
		}
	}

	return 0;
}

bool InventoryComponent::isInventoryFull()
{
	if (_items.size() >= _maxItemNumber)
		return true;
	else
		return false;
}

bool InventoryComponent::isItemInInventory(std::string entTypeID)
{
	for (auto& it : _items)
		if (it->getTypeID() == entTypeID)
			return true;

	return false;
}

void InventoryComponent::printInventory()
{
	for (auto& it : _items)
	{
		std::cout << it->getName() << std::endl;
	}
}*/


SpawnerComponent::SpawnerComponent()
	:Component(ComponentType::Spawner)
{
	
}

void SpawnerComponent::readIn(const sol::table& compTable) {

}


AIComponent::AIComponent(/*std::string npcType*/)
	:Component(ComponentType::AI)
{
	_targetPosition = sf::Vector2f(704, 352);	//TEMP
	_guardPosition = sf::Vector2f(300.f, 300.f);
	//_enemy = nullptr;
}

void AIComponent::readIn(const sol::table& compTable) {
	//std::string AIType = compTable["npcType"];

	/*if (compTable["PatrolPositions"] != sol::nil) {
		sol::table patrolTable = compTable["PatrolPositions"];

		/*for (auto& patrolData : getKeyValueMap(table)) {
			luabridge::LuaRef positionTable = patrolData.second;
			std::cout << "TESTING" << patrolData.first << std::endl;
			sf::Vector2f newPatrolPos = sf::Vector2f(positionTable[1].cast<float>(), positionTable[2].cast<float>());
			this->get<AIComponent>()->_patrolPositions.push_back(newPatrolPos);
		}*/

		/*if (patrolTable["position1"].isTable()) {
			luabridge::LuaRef positionTable = patrolTable["position1"];
			sf::Vector2f newPatrolPos = sf::Vector2f(positionTable[1].cast<float>(), positionTable[2].cast<float>());
			_patrolPositions.push_back(newPatrolPos);

			if (patrolTable["position2"].isTable()) {
				luabridge::LuaRef positionTable = patrolTable["position2"];
				newPatrolPos = sf::Vector2f(positionTable[1].cast<float>(), positionTable[2].cast<float>());
				_patrolPositions.push_back(newPatrolPos);

				if (patrolTable["position3"].isTable()) {
					luabridge::LuaRef positionTable = patrolTable["position3"];
					newPatrolPos = sf::Vector2f(positionTable[1].cast<float>(), positionTable[2].cast<float>());
					_patrolPositions.push_back(newPatrolPos);

					if (patrolTable["position4"].isTable()) {
						luabridge::LuaRef positionTable = patrolTable["position4"];
						newPatrolPos = sf::Vector2f(positionTable[1].cast<float>(), positionTable[2].cast<float>());
						_patrolPositions.push_back(newPatrolPos);
					}
				}
			}
		}
	}*/
}


DialogueComponent::DialogueComponent(/*unsigned int baseConvoID*/)
	:Component(ComponentType::Dialogue),
	_baseConversationID(0)
{

}

void DialogueComponent::readIn(const sol::table& compTable) {
	//unsigned int conversationID = compTable["conversationID"].cast<unsigned int>();
	//_baseConversationID = conversationID;
}


QuestComponent::QuestComponent(/*std::string questDocPath*/)
	:Component(ComponentType::Quest)
{
	
}

void QuestComponent::readIn(const sol::table& compTable) {
	std::string questDataPath = compTable["questDataPath"];
	pugi::xml_parse_result result = _questDoc.load_file(questDataPath.c_str());
}


ScriptStateMachineComponent::ScriptStateMachineComponent()
	:Component(ComponentType::ScriptState)
{
	
}

void ScriptStateMachineComponent::readIn(const sol::table& compTable) {

}


ScheduleComponent::ScheduleComponent()
	:Component(ComponentType::Schedule),
	_changingTask(true),
	_currentTaskIndex(0)
{

}
 
void ScheduleComponent::readIn(const sol::table& compTable) {

}

void ScheduleComponent::advanceCurrentTask() {
	this->_currentTaskIndex++;
	this->_changingTask = true;
}

void ScheduleComponent::resetSchedule() {
	this->_currentTaskIndex = 0;
	this->_changingTask = true;
}

void ScheduleComponent::endSchedule() {
	this->_currentTaskIndex = _tasks.size() + 1;
}


TimerComponent::TimerComponent()
	:Component(ComponentType::Timer)
{
	_timers.clear();
}

void TimerComponent::readIn(const sol::table& compTable) {
	//should loop through all timers and add their tick timings 

	for (int i = 0; i < compTable.size(); ++i)
	{
		sol::table timerTable = compTable[i + 1];
		if (timerTable != sol::nil) {
			std::string name = timerTable["name"];
			float tickTime = timerTable["tickTime"];
			bool repeat = timerTable["repeats"];

			_timers[name]._elapsedTime = 0;
			_timers[name]._tickTime = tickTime;
			_timers[name]._active = false;
			_timers[name]._repeat = repeat;
			_timers[name]._tableIndex = i + 1;
		}
	}
}


InteractComponent::InteractComponent(/*sf::Vector2f position, sf::Vector2f localPos, sf::Vector2f size*/)
	:Component(ComponentType::Interact)
{

}

void InteractComponent::readIn(const sol::table& compTable, PositionComponent* posComp, TextureComponent* texComp) {
	if (compTable["interactRect"] != sol::nil)
	{
		sol::table interactRectTable = compTable["interactRect"];
		sf::Vector2f colRectSize;
		sf::Vector2f localColRectPos;

		localColRectPos.x = interactRectTable[1];
		localColRectPos.y = interactRectTable[2];

		colRectSize.x = interactRectTable[3];
		colRectSize.y = interactRectTable[4];

		//apply to vars
		_interactRect.left = posComp->_position.x + localColRectPos.x;
		_interactRect.top = posComp->_position.y + localColRectPos.y;
		_interactRect.width = colRectSize.x;
		_interactRect.height = colRectSize.y;

		_localInteractRectOffset = localColRectPos;
	}

	//scale if necessary
	if (texComp != nullptr) {
		_interactRect.width *= texComp->_sprite.getScale().x;
		_interactRect.height *= texComp->_sprite.getScale().y;
	}
}

LightingComponent::LightingComponent() : Component(ComponentType::Lighting)
{

}

void LightingComponent::readIn(const sol::table& compTable, PositionComponent* posComp, LightManager* lightingManager, EntityID entId) {
	std::string texPath = compTable["texture"];
	float radius = compTable["radius"];

	sf::Color colour;

	sol::table colourTable = compTable["colour"];
	if (colourTable != sol::nil) {
		colour = sf::Color(colourTable["red"], colourTable["green"], colourTable["blue"]);
	}

	_positionRoot = posComp->_position;

	//add dynamic light to the lighting manager
	lightingManager->addLight(_positionRoot, texPath, colour, radius, true, entId);
	std::cout << "added dynamic light" << std::endl;
}