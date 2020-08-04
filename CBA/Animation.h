#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <SFML/Graphics.hpp>
#include <sstream>

class SpriteSheet;

class Animation {
	friend class SpriteSheet;
public:
	Animation();
	virtual ~Animation();

	bool isInAction();
	void setFrame(const unsigned int& frame);
	void setSpriteSheet(SpriteSheet*);

	void play();
	void pause();
	void stop();
	void reset();

	void setLooping(const bool);

	virtual void update(const float&);

	friend std::stringstream& operator >>(std::stringstream& ss, Animation& a) {
		a.readIn(ss);
		return ss;
	}
protected:
	virtual void frameStep() = 0;
	virtual void cropSprite() = 0;
	virtual void readIn(std::stringstream&) = 0;

	unsigned int _startFrame;
	unsigned int _endFrame;
	unsigned int _currentFrame;
	unsigned int _frameRow;
	unsigned int _frameActionStart;
	unsigned int _frameActionEnd;

	float _frameTime;
	float _elapsedTime;
	bool _loop;
	bool _playing;
	std::string _name;
	SpriteSheet* _spriteSheet;
};

#endif