#ifndef ENTITY_MESSAGE_H
#define ENTITY_MESSAGE_H

enum class EntityMessage {
	Move, Is_Moving, State_Changed, Direction_Changed,
	Switch_State, Attack_Action, Dead, Interact, PrimaryFire
};

#endif