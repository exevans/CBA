#ifndef MESSAGE_H
#define MESSAGE_H

#include <SFML/Graphics.hpp>
#include <utility>

using MessageType = unsigned int;
struct TwoFloats { float _x; float _y; };

struct Message {
	Message(const MessageType& type) : _mType(type) {};
	
	MessageType _mType;
	int _sender;
	int _receiver;

	union {
		TwoFloats twoFloats;
		int _int;
		bool _bool;
	};
};

#endif