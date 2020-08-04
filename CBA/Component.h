#ifndef COMPONENT_H
#define COMPONENT_H

#include <sol.hpp>
#include <SFML/Graphics.hpp>
#include <bitset>
#include <string>
#include <memory>

//#include <LuaBridge.h>

extern "C"
{
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#include "Direction.h"
#include "pugixml.hpp"
#include "ComponentType.h"
#include "SpriteSheet.h"

class Entity;
class TextureManager;
class LightManager;

class Component
{
public:
	void setOwnerID(unsigned int);
	void setOwnerEntity(std::shared_ptr<Entity>);
	unsigned int getOwnerID() const;
	Entity* getOwnerEntity() const;
	const _Uint32t getComponentID() const;
	const ComponentType getComponentType();

	virtual void readIn(const sol::table&) = 0;

	void setActive(bool);
	bool isActive() const;

	virtual ~Component();

protected:
	Component(const ComponentType&);
	std::weak_ptr<Entity> _ownerEntity;
	unsigned int _entityID;
	_Uint32t _componentID;
	bool _active = true;
	ComponentType _compType;
};


struct HealthComponent : public Component
{
public:
	HealthComponent();
	void readIn(const sol::table&) override;

	int _health = 100;
	bool _alive = true;
};


struct PositionComponent : public Component
{
public:
	PositionComponent();
	void readIn(const sol::table&) override;
	void moveBy(const sf::Vector2f);

	sf::Vector2f _position;

	std::string _destinationCellId;
	unsigned int _destinationTriggerId = 0;
};


struct MovementComponent : public Component
{
public:
	MovementComponent();
	void readIn(const sol::table&) override;
	void accelerate(const sf::Vector2f&);
	void addVelocity(const sf::Vector2f&);
	void setVelocity(sf::Vector2f);
	void applyFriction(const sf::Vector2f&);
	void move(const Direction&);
	void setDirection(const Direction&);

	sf::Vector2f _prevVelocity;
	sf::Vector2f _velocity;
	sf::Vector2f _speed;
	float _maxVelocity;
	sf::Vector2f _acceleration;
	Direction _direction;
	bool _experiencesFriction;
};


struct TextureComponent : public Component
{
public:
	TextureComponent ();
	void readIn(const sol::table&) override;

	std::string _texturePath;
	sf::Vector2u _textureSize;
	sf::Sprite _sprite;

	unsigned int _zOrder = 1;
};

struct SpriteSheetComponent : public Component
{
public:
	SpriteSheetComponent();
	void readIn(const sol::table&) override;
	void create(TextureManager*, std::string&, const sol::table&);

	std::string _sheetName;
	SpriteSheet* _spriteSheet;
};

struct InputComponent : public Component
{
public:
	InputComponent();
	void readIn(const sol::table&) override;

	bool _controllable;
};

struct CollisionComponent : public Component
{
public:
	CollisionComponent();
	//CollisionComponent(TextureComponent*);
	void readIn(const sol::table&) {}
	void readIn(const sol::table&, PositionComponent* = nullptr, TextureComponent* = nullptr);

	bool collisionContainsPoint(sf::Vector2f);

	sf::FloatRect _colRect;
	sf::Vector2f _localColRectOffset;

	unsigned int collidedEntityID = -1;
};


struct AnimationComponent : public Component
{
public:
	AnimationComponent();
	void readIn(const sol::table&) override;
	//void stopCurrentAnimation();
	//void playAnimation(const sol::table&, const bool);
	//sf::Vector2u _imageSize;
	//sf::Vector2u _currentTexPos;

	/*sf::Vector2u _startFramePos;
	sf::Vector2u _endFramePos;*/

	/*sf::Vector2f _spriteSize;

	unsigned int _currentFrame;
	unsigned int _startFrame;
	unsigned int _endFrame;
	//unsigned int _currentRow;
	unsigned int _startRow;
	//unsigned int _endRow;

	bool _playing;
	bool _loop;
	float _animationRate;
	float _elapsedTime;*/

	SpriteSheet _spriteSheet;
};

struct ItemComponent : public Component
{
public:
	ItemComponent();
	void readIn(const sol::table&) override;
};

struct InventoryComponent : public Component
{
public:
	InventoryComponent();
	void readIn(const sol::table&) override;

	/*void addEntity(std::shared_ptr<Entity>);
	void removeEntity(unsigned int);
	void removeEntity(std::string);
	void clearInventory();
	Entity* getItem(unsigned int);
	Entity* getEquipedItem();
	void setEquipedItem(unsigned int);
	void setEquipedItemID(unsigned int);
	unsigned int getEquipedItemVectVal();
	bool isInventoryFull();
	bool isItemInInventory(std::string);
	void printInventory();

	unsigned int _maxItemNumber;
	std::vector<std::shared_ptr<Entity>>_items;
	unsigned int _equipedItemID;*/
};

struct SpawnerComponent : public Component
{
public:
	SpawnerComponent();
	void readIn(const sol::table&) override;

	std::string _spawnScriptPath;
	std::string _spawnTableName;

	bool _spawning = false;
};

struct AIComponent : public Component
{
public:
	AIComponent();
	void readIn(const sol::table&) override;

	sf::Vector2f _guardPosition;
	sf::Vector2f _targetPosition;
	//std::shared_ptr<Entity> _enemy;

	std::vector<sf::Vector2f> _routeNodes;
	unsigned int _currentRouteNode = 0;
	std::vector<sf::Vector2f> _patrolPositions;
	unsigned int _currentPatrolPoint = 0;
	bool _firstPointCalculated = false;	//so that the root is calculated first

	bool calculatingRoot = false;

	bool _viewOfPlayer = false;
};

struct DialogueComponent : public Component
{
public:
	DialogueComponent();
	void readIn(const sol::table&) override;

	unsigned int _baseConversationID = 0;	//set to an entitys default convo ID
	unsigned int _otherEntity;
	bool _inConversation = false;
};

struct QuestComponent : public Component
{
public:
	QuestComponent();
	void readIn(const sol::table&) override;

	pugi::xml_document _questDoc;
	pugi::xml_node _QuestRoot;

	pugi::xml_node _convoRoot;	//for observer system	should delete
};

struct ScriptStateMachineComponent : public Component
{
public:
	ScriptStateMachineComponent();
	void readIn(const sol::table&) override;

	std::string _currentState;
	std::string _newState;
	bool _changingState = false;
};



struct ScheduleData {
	ScheduleData() {};
	ScheduleData(int data) : _int(data) {};
	ScheduleData(sf::Vector2f& data) : _vect2f(data) {};
	ScheduleData(std::string& data) : _str(data) {};

	int _int;
	sf::Vector2f _vect2f;
	std::string _str;
};

struct ScheduleComponent : public Component {
public:
	ScheduleComponent();
	void readIn(const sol::table&) override;
	void advanceCurrentTask();
	void resetSchedule();
	void endSchedule();

	unsigned int _currentTaskIndex;
	std::vector<std::pair<std::string, ScheduleData>> _tasks;
	bool _changingTask;
};

struct TimerComponent : public Component
{
	struct Timer {
		float _elapsedTime;
		float _tickTime;
		bool _active;
		bool _repeat;
		int _tableIndex;
	};
public:
	TimerComponent();
	void readIn(const sol::table&) override;

	//current time passed followed by tick time
	std::map<std::string, Timer> _timers;
};

struct InteractComponent : public Component
{
public:
	InteractComponent();
	void readIn(const sol::table&) {}
	void readIn(const sol::table&, PositionComponent* = nullptr, TextureComponent* = nullptr);

	sf::FloatRect _interactRect;
	sf::Vector2f _localInteractRectOffset;
};

struct LightingComponent : public Component
{
public:
	LightingComponent();
	void readIn(const sol::table&) {}
	void readIn(const sol::table&, PositionComponent*, LightManager*, unsigned int);

	sf::Vector2f _positionOffset;
	sf::Vector2f _positionRoot;
	std::string _texture;
	sf::Color _colour;
	float _radius;
};

#endif