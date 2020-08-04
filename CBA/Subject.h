#ifndef SUBJECT_H
#define SUBJECT_H

#include <iostream>
#include <vector>
#include <map>
#include <iterator>

class Observer;
class Entity;

class Subject
{
public:
	~Subject();
	void addObserver(Observer&);
	void removeObserver(Observer&);
	void notify(Entity*, std::string);
	void notify(Entity*, std::string, unsigned int);	//for quests
	void notify(Entity*, std::string, unsigned int, unsigned int);	//for quests
private:
	std::vector<Observer*> _observers;
};

#endif