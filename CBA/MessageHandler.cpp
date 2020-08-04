#include "MessageHandler.h"

bool MessageHandler::subscribe(const EntityMessage& type, Observer* observer) {
	return _communicators[type].addObserver(observer); 
}

bool MessageHandler::unsubscribe(const EntityMessage& type, Observer* observer) {
	return _communicators[type].removeObserver(observer);
}

void MessageHandler::dispatch(const Message& msg) {
	auto itr = _communicators.find((EntityMessage)msg._mType);
	if (itr == _communicators.end()) {
		return;
	}
	itr->second.broadcast(msg);
}