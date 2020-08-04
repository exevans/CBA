#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>
#include <string>

#include "Message.h"

class Entity;

class Observer
{
public:
	virtual void onNotify(Entity*, std::string);
	virtual void onNotify(Entity*, std::string, unsigned int);
	virtual void onNotify(Entity*, std::string, unsigned int, unsigned int);
	virtual void onNotify(const Message&) {}; //will be fully virtual
};

#endif