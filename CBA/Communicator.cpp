#include "Communicator.h"
#include "Observer.h"

Communicator::Communicator() {

}

Communicator::~Communicator() {
	_observers.clear();
}

bool Communicator::addObserver(Observer* observer) {
	if (hasObserver(observer)) {
		return false;
	}
	_observers.emplace_back(observer);
	return true;
}

bool Communicator::removeObserver (Observer* observer) {
	auto itr = std::find(_observers.begin(), _observers.end(), observer);
	if (itr == _observers.end()) {
		return false;
	}
	_observers.erase(itr);
	return true;
}

void Communicator::broadcast(const Message& message) {
	for (auto it : _observers) {
		it->onNotify(message);
	}
}

bool Communicator::hasObserver(const Observer* observer) {
	auto itr = std::find(_observers.begin(), _observers.end(), observer);
	if (itr == _observers.end()) {
		return false;
	}

	return true;
}
