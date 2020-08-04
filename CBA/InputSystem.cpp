#include "InputSystem.h"
#include "Entity.h"
#include "Component.h"
#include "Direction.h"
#include "LuaManager.h"
#include "EntityMessage.h"
#include "EntityManager.h"
#include <iostream>
#include "DialogueManager.h"
#include "SystemManager.h"

InputSystem::InputSystem(SystemManager* sysManager) : 
	System(sysManager, SystemType::Input)
{
	addRequiredComponentType(ComponentType::Input);
	addRequiredComponentType(ComponentType::Movement);

	_systemManager->getMessageHandler()->subscribe(EntityMessage::Move, this);
	_systemManager->getMessageHandler()->subscribe(EntityMessage::Interact, this);
	_systemManager->getMessageHandler()->subscribe(EntityMessage::PrimaryFire, this);

	//TEMP GUI
	/*_equipedItemTextBox.setSize(sf::Vector2f(100.f, 30.f));
	_equipedItemTextBox.setFillColor(sf::Color::Black);
	_equipedItemTextBox.setOutlineColor(sf::Color::White);
	_equipedItemTextBox.setOutlineThickness(2.f);
	_equipedItemTextBox.setPosition(0.f, 0.f);

	_equipedItemText.setCharacterSize(22);
	_equipedItemText.setFillColor(sf::Color::White);
	_equipedItemText.setPosition(_equipedItemTextBox.getPosition());

	font.loadFromFile("Assets/Fonts/BERNHC.ttf");
	_equipedItemText.setFont(font);
	_equipedItemText.setString("Fists");*/
}

void InputSystem::update(float deltaTime)
{
	/*auto& inputComps = _components.find(std::type_index(typeid(InputComponent)))->second;
	for (std::map<unsigned int, Component*>::iterator it = inputComps.begin(); it != inputComps.end(); ++it)
	{
		auto inputComp = this->get<InputComponent>(it->first);
		auto moveComp = this->get<MovementComponent>(it->first);

		//reset Movement to 0
		moveComp->_movement = sf::Vector2f(0.f, 0.f);

		if (!it->second->getOwnerEntity()->hasComponent(std::type_index(typeid(DialogueComponent))) || !DialogueManager::_inDialogue)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				moveComp->_movement.x = moveComp->_speed;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				moveComp->_movement.x = -moveComp->_speed;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				moveComp->_movement.y = -moveComp->_speed;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				moveComp->_movement.y = moveComp->_speed;
		}
		if (KeyBinder::isEventPending("Interact") && !DialogueManager::_inDialogue)
			inputComp->InteractPressed = true;
		else
			inputComp->InteractPressed = false; 

		//temp joystick
		if (sf::Joystick::isConnected(1))
		{
			if (sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::X) > 15.f || sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::X) < -15.f || sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::Y) > 15.f || sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::Y) < -15.f)
				moveComp->_movement = sf::Vector2f(sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::X), sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::Y));

			if (sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::R) > 20.f || sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::R) < -20.f || sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::U) > 20.f || sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::U) < -20.f)
			{
				sf::Vector2f mouseMovement = sf::Vector2f(sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::U), sf::Joystick::getAxisPosition(1, sf::Joystick::Axis::R));

				if (mouseMovement.x != 0 && mouseMovement.y != 0)
				{
					float speed = 15;

					float k = (speed / (sqrt((mouseMovement.x * mouseMovement.x) + (mouseMovement.y * mouseMovement.y))));
					mouseMovement.x *= k;
					mouseMovement.y *= k;
				}

				sf::Mouse::setPosition(sf::Mouse::getPosition() + sf::Vector2i(mouseMovement));
			}
		}

		//Dialogue
		if (it->second->getOwnerEntity()->hasComponent(std::type_index(typeid(DialogueComponent))) && DialogueManager::_inDialogue == true)
		{

		}
		else if (it->second->getOwnerEntity()->hasComponent(std::type_index(typeid(InventoryComponent))))		//Inventory
		{
			auto inventComp = it->second->getOwnerEntity()->get<InventoryComponent>();

			if (KeyBinder::isEventPending("Inventory"))
			{
				inventComp->printInventory();
			}

			if (KeyBinder::isEventPending("Activate"))
			{
				it->second->getOwnerEntity()->get<InputComponent>()->ActivatePressed = true;

				if (inventComp->_items.size() > 0)
				{
					luabridge::LuaRef table = luabridge::getGlobal(GameManager::L, it->second->getOwnerEntity()->get<InventoryComponent>()->getItem(it->second->getOwnerEntity()->get<InventoryComponent>()->_equipedItemID)->getScriptTable().c_str());
					luabridge::LuaRef compTable = table["ItemComponent"];
					luabridge::LuaRef activateFunc = compTable["activate"];

					//if (activateFunc.isFunction())
						//activateFunc(LuaEntityHandle(it->second->getOwnerEntity()), LuaEntityHandle(inventComp->getItem(inventComp->_equipedItemID)));
				}
			}
			else
				inputComp->ActivatePressed = false;

			if (KeyBinder::isEventPending("NextItem"))
				inventComp->setEquipedItem(inventComp->getEquipedItemVectVal() + 1);
			else if (KeyBinder::isEventPending("PreviousItem"))
				inventComp->setEquipedItem(inventComp->getEquipedItemVectVal() - 1);

			if (KeyBinder::isEventPending("Option1"))
				inventComp->setEquipedItem(0);
			else if (KeyBinder::isEventPending("Option2"))
				inventComp->setEquipedItem(1);
			else if (KeyBinder::isEventPending("Option3"))
				inventComp->setEquipedItem(2);
			else if (KeyBinder::isEventPending("Option4"))
				inventComp->setEquipedItem(3);
			else if (KeyBinder::isEventPending("Option5"))
				inventComp->setEquipedItem(4); 

			//_equipedItemText.setString(inventComp->getEquipedItem()->getName());
		}
	}*/
}

void InputSystem::handleEvent(const EntityID& ent, const EntityEvent& entEvent) {
	MovementComponent* movComp = _systemManager->getEntityManager()->getComponent<MovementComponent>(ent, ComponentType::Movement);

	switch (entEvent) {
	case EntityEvent::Moving_Up:
		movComp->move(Direction::Up);
		break;
	case EntityEvent::Moving_Left:
		movComp->move(Direction::Left);
		break;
	case EntityEvent::Moving_Down:
		movComp->move(Direction::Down);
		break;
	case EntityEvent::Moving_Right:
		movComp->move(Direction::Right);
		break;
	case EntityEvent::PrimaryFiring:	//WIP
		EntityManager* entManager = _systemManager->getEntityManager();
		sol::table HPTable = entManager->getLuaManager()->getLuaState()[_systemManager->getEntityManager()->getEntityInfo(ent).scriptTable.c_str()];	//setup lua link
		sol::function onPrimaryFired = HPTable["onPrimaryFired"];
		if (onPrimaryFired.valid()) {
			onPrimaryFired(LuaEntityHandle(_systemManager->getEntityManager(), ent));
		}
		break;
	}
}

void InputSystem::onNotify(const Message& message) {

	EntityManager* entManager = _systemManager->getEntityManager();
	InputComponent* inComp = entManager->getComponent<InputComponent>(message._receiver, ComponentType::Input);
	//entity must be able to be controlled at the time
	if (!inComp->_controllable) { return; }

	switch (message._mType) {
	case (int)EntityMessage::Move:
		if (message._int == (int)Direction::Up)
			_systemManager->addEvent(message._receiver, EventID(EntityEvent::Moving_Up));
		if (message._int == (int)Direction::Left)
			_systemManager->addEvent(message._receiver, EventID(EntityEvent::Moving_Left));
		if (message._int == (int)Direction::Down)
			_systemManager->addEvent(message._receiver, EventID(EntityEvent::Moving_Down));
		if (message._int == (int)Direction::Right)
			_systemManager->addEvent(message._receiver, EventID(EntityEvent::Moving_Right));
		break;
	case (int)EntityMessage::Interact:
		_systemManager->addEvent(message._receiver, EventID(EntityEvent::Interacting));
		break;
	case (int)EntityMessage::PrimaryFire:
		_systemManager->addEvent(message._receiver, EventID(EntityEvent::PrimaryFiring));
		break;
	}

}


