#include "Subject.h"
#include "Observer.h"
#include "Entity.h"

Subject::~Subject()
{
	this->_observers.clear();
}

void Subject::addObserver(Observer& observer)
{
	_observers.push_back(&observer);
}

void Subject::removeObserver(Observer& observer)
{
	for (size_t i = 0; i < this->_observers.size(); ++i)
	{
		if (_observers[i] == (&observer))
			_observers.erase(_observers.begin() + i);
	}
}

void Subject::notify(Entity* entity, std::string eventType)
{
	for (std::vector<Observer*>::iterator it = _observers.begin(); it != _observers.end(); ++it)
	{
		(*it)->onNotify(entity, eventType);
	}
}

void Subject::notify(Entity* entity, std::string eventType, unsigned int num1)
{
	for (std::vector<Observer*>::iterator it = _observers.begin(); it != _observers.end(); ++it)
	{
		(*it)->onNotify(entity, eventType, num1);
	}
}

void Subject::notify(Entity* entity, std::string eventType, unsigned int num1, unsigned int num2)
{
	for (std::vector<Observer*>::iterator it = _observers.begin(); it != _observers.end(); ++it)
	{
		(*it)->onNotify(entity, eventType, num1, num2);
	}
}
