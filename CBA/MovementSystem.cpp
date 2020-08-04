#include "MovementSystem.h"
#include "SystemManager.h"
#include "Component.h"
#include "Entity.h"
#include <iostream>

MovementSystem::MovementSystem(SystemManager* sysManager)
	:System(sysManager, SystemType::Movement)
{
	addRequiredComponentType(ComponentType::Position);
	addRequiredComponentType(ComponentType::Movement);
}

void MovementSystem::update(float deltaTime)
{
	EntityManager* entManager = _systemManager->getEntityManager();
	for (auto& ent : _entities) {
		PositionComponent* posComp = entManager->getComponent<PositionComponent>(ent, ComponentType::Position);
		MovementComponent* moveComp = entManager->getComponent<MovementComponent>(ent, ComponentType::Movement);

		movementStep(deltaTime, moveComp, posComp);
		posComp->moveBy(moveComp->_velocity * deltaTime);

		//standadize movement
		//sf::Vector2f movement(moveComp->_movement);
		//posComp->moveBy(movement);

		/*if (movement.x != 0 || movement.y != 0)
		{
			//sets moving
			moveComp->_moving = true;

			//NORMALISE MOVEMENT
			if (movement.x != 0 && movement.y != 0)
			{
				const float speed = moveComp->_speed;

				float k = (speed / (sqrt((movement.x * movement.x) + (movement.y * movement.y))));
				movement.x *= k;
				movement.y *= k;
			}

			//sets animation direction
			if (abs(movement.x) > abs(movement.y))
			{
				if (movement.x > 0)
				{
					//LuaEntityHandle(moveComp->getOwnerEntity()).setAnimation("walkRight");
				}
				else if (movement.x < 0)
				{
					//LuaEntityHandle(moveComp->getOwnerEntity()).setAnimation("walkLeft");
				}
			}
			else
			{
				if (movement.y > 0)
				{
					//LuaEntityHandle(moveComp->getOwnerEntity()).setAnimation("walkDown");
				}
				else if (movement.y < 0)
				{
					//LuaEntityHandle(moveComp->getOwnerEntity()).setAnimation("walkUp");
				}
			}
		}
		else
		{
			//sets not moving
			moveComp->_moving = false;
		}*/
	}
}

void MovementSystem::movementStep(float delta, MovementComponent* movComp, PositionComponent* posComp) {
	//sf::Vector2f friction(movComp->_speed.x/2 + std::abs(movComp->_velocity.x/2) * 1, movComp->_speed.y/2 + std::abs(movComp->_velocity.y/2) * 1);
	//sf::Vector2f friction(5 * std::exp(2 * movComp->_velocity.x), 5 * std::exp(2 * movComp->_velocity.y));
	sf::Vector2f friction(1, 1);
	/*movComp->addVelocity(movComp->_acceleration * delta); 

	if (movComp->_acceleration.x == 0 || (movComp->_acceleration.x > 0 && movComp->_velocity.x < 0) || (movComp->_acceleration.x < 0 && movComp->_velocity.x > 0)) {
		friction.x = movComp->_speed.x * 4 + 50;
	}
	if (movComp->_acceleration.y == 0) {
		friction.y = movComp->_speed.y * 4 + 50;
	}
	movComp->_acceleration = sf::Vector2f(0.f, 0.f);
	movComp->applyFriction(friction * delta);*/

	if (movComp->_experiencesFriction) {
		sf::Vector2f fricVel(friction.x * movComp->_prevVelocity.x, friction.y * movComp->_prevVelocity.y);
		sf::Vector2f frictionAppliedAccel = movComp->_acceleration - fricVel;
		sf::Vector2f potentialVel = movComp->_velocity + frictionAppliedAccel;

		/*if (movComp->_acceleration.y == 0 && movComp->_velocity.y == 0) {
			frictionAppliedAccel.y = 0;
			movComp->_velocity.y = 0;
		}
		if (movComp->_acceleration.x == 0 && movComp->_velocity.x == 0) {
			frictionAppliedAccel.x = 0;
			movComp->_velocity.x = 0;
		}*/

		/*if ((potentialVel.y > 0 && movComp->_prevVelocity.y < 0) || (potentialVel.y < 0 && movComp->_prevVelocity.y > 0) || potentialVel.y == 0) {
			frictionAppliedAccel.y = 0;
			movComp->_velocity.y = 0;
		}*/
		/*if ((potentialVel.x > 0 && movComp->_prevVelocity.x < 0) || (potentialVel.x < 0 && movComp->_prevVelocity.x > 0)) {
			frictionAppliedAccel.x = 0;
			movComp->_velocity.x = 0;
		}*/

		movComp->_velocity += frictionAppliedAccel;
		//movComp->addVelocity(frictionAppliedAccel);
		//movComp->addVelocity(movComp->_acceleration);
		//movComp->applyFriction(fricVel);

		//movComp->applyFriction(fricVel);
	}
	
	//calc total mag of vel
	float magnitude = sqrt((movComp->_velocity.x * movComp->_velocity.x) + (movComp->_velocity.y * movComp->_velocity.y));

	//needs to be standardised
	if (magnitude > movComp->_maxVelocity) {
		//standardise velocity
		movComp->setVelocity(sf::Vector2f(movComp->_velocity.x / magnitude * movComp->_maxVelocity, movComp->_velocity.y / magnitude * movComp->_maxVelocity));
	}

	//reset values
	movComp->_acceleration = sf::Vector2f(0.f, 0.f);
	movComp->_prevVelocity = movComp->_velocity;
}

void MovementSystem::handleEvent(const EntityID& ent, const EntityEvent& entEvent) {
	EntityManager* entManager = _systemManager->getEntityManager();

	switch (entEvent) {
	case EntityEvent::Colliding_X:
		stopEntity(ent, Axis::X);
		break;
	case EntityEvent::Colliding_Y:
		stopEntity(ent, Axis::Y);
		break;
	case EntityEvent::Moving_Down:
		//if (entManager->getComponent<MovementComponent>(ent, ComponentType::Movement)->_velocity.x == 0)
			setDirection(ent, Direction::Down);
		break;
	case EntityEvent::Moving_Up:
		//if (entManager->getComponent<MovementComponent>(ent, ComponentType::Movement)->_velocity.x == 0)
			setDirection(ent, Direction::Up);
		break;
	case EntityEvent::Moving_Left:
		setDirection(ent, Direction::Left);
		break;
	case EntityEvent::Moving_Right:
		setDirection(ent, Direction::Right);
		break;
	}
}

void MovementSystem::stopEntity(const EntityID& ent, const Axis& axis) {
	MovementComponent* moveComp = _systemManager->getEntityManager()->getComponent<MovementComponent>(ent, ComponentType::Movement);
	if (axis == Axis::X) {
		moveComp->setVelocity(sf::Vector2f(0.f, moveComp->_velocity.y));
	}
	else if (axis == Axis::Y) {
		moveComp->setVelocity(sf::Vector2f(moveComp->_velocity.x, 0.f));
	}
}

void MovementSystem::setDirection(const EntityID& ent, const Direction& dir) {
	Direction dir2 = dir;
	MovementComponent* moveComp = _systemManager->getEntityManager()->getComponent<MovementComponent>(ent, ComponentType::Movement);
	if (std::abs(moveComp->_velocity.x) > std::abs(moveComp->_velocity.y)) {
		if (moveComp->_acceleration.x > 0) {
			dir2 = Direction::Right;
		} 
		else {
			dir2 = Direction::Left;
		}
			
	}
	else {
		if (moveComp->_acceleration.y > 0) {
			dir2 = Direction::Down;
		}
		else {
			dir2 = Direction::Up;
		}
	}

	//detect whether the direction has changed
	if (dir2 == moveComp->_direction) { return; }
	moveComp->setDirection(dir2);

	Message mess = Message(MessageType(EntityMessage::Direction_Changed));
	mess._receiver = ent;
	mess._int = (int)moveComp->_direction;
	_systemManager->getMessageHandler()->dispatch(mess);
}

void MovementSystem::notify(const Message& message) {
	EntityManager* entManager = _systemManager->getEntityManager();
	EntityMessage m = (EntityMessage)message._mType;
	switch (m) {
	case EntityMessage::Is_Moving:
		if (!hasEntity(message._receiver)) { return; }
		MovementComponent* movComp = entManager->getComponent<MovementComponent>(message._receiver, ComponentType::Movement);
		if (movComp->_velocity != sf::Vector2f(0, 0)) { return; }

		_systemManager->addEvent(message._receiver, (EventID)EntityEvent::Became_Idle);
		break;
	}
}