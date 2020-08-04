#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <unordered_map>

#include "Communicator.h"
#include "Message.h"
#include "EntityMessage.h"

using Subscriptions = std::unordered_map<EntityMessage, Communicator>;

class MessageHandler {
public:
	bool subscribe(const EntityMessage&, Observer*);
	bool unsubscribe(const EntityMessage&, Observer*);
	void dispatch(const Message&);

private:
	Subscriptions _communicators;
};

#endif