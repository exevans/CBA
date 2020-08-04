#include "Animation.h"
#include "SpriteSheet.h"

Animation::Animation() : _currentFrame(0),
	_startFrame(0),
	_endFrame(0),
	_frameTime(0),
	_elapsedTime(0.f),
	_frameRow(0),
	_frameActionStart(-1),
	_frameActionEnd(-1),
	_loop(true),
	_playing(false)
{

}

Animation::~Animation() {

}

bool Animation::isInAction() {
	if (_frameActionStart == -1 || _frameActionEnd == -1) {
		return true;
	}

	return (_currentFrame >= _startFrame && _currentFrame <= _endFrame);
}

void Animation::setFrame(const unsigned int& frame) {
	if (frame >= _startFrame && frame <= _endFrame)
	_currentFrame = frame;
}

void Animation::setSpriteSheet(SpriteSheet* sSheet) {
	_spriteSheet = sSheet;
}

void Animation::play() {
	_playing = true;
}

void Animation::pause() {
	_playing = false;
}

void Animation::stop() {
	_playing = false;
	reset();
}

void Animation::reset() {
	_currentFrame = _startFrame;
	_elapsedTime = 0.f;
	cropSprite();
}

void Animation::setLooping(const bool loop) {
	_loop = loop;
}

void Animation::update(const float& deltaTime) {
	if (!_playing) { return; }
	_elapsedTime += deltaTime;
	if (_elapsedTime > _frameTime) {
		frameStep();
		cropSprite();
		_elapsedTime = 0;
	}
}