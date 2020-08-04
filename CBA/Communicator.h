#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <vector>

#include "Message.h"

class Observer;

class Communicator {
public:
	Communicator();
	~Communicator();
	bool addObserver(Observer*);
	bool removeObserver(Observer*);
	bool hasObserver(const Observer*);
	void broadcast(const Message&);

private:
	std::vector<Observer*> _observers;
};

#endif