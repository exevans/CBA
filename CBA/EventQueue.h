#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <queue>

using EventID = unsigned int;

class EventQueue {
public:
	void addEvent(const EventID& event) { _queue.push(event); };
	void clear() {
		while (!_queue.empty()) {
			_queue.pop();
		}
	};

	bool processEvents(EventID& id) {
		if (_queue.empty()) {
			return false;
		}
		id = _queue.front();
		_queue.pop();
		return true;
	}

private:
	std::queue<EventID> _queue;
};

#endif