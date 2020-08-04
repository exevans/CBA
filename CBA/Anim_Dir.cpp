#include "Anim_Dir.h"
#include "SpriteSheet.h"

void Anim_Dir::readIn(std::stringstream& ss) {
	ss >> _startFrame >> _endFrame >> _frameRow >> _frameTime >> _frameActionStart >> _frameActionEnd;
}

void Anim_Dir::frameStep() {
	if (_currentFrame <= _endFrame) {
		++_currentFrame;
	}

	if (_startFrame < _endFrame && _currentFrame > _endFrame) {
		if (_loop) {
			_currentFrame = _startFrame;
			return;
		}
		else {
			_currentFrame = _endFrame;
			pause();
		}
	}
}

void Anim_Dir::cropSprite() {
	sf::IntRect rect;
	rect.left = _spriteSheet->getSpriteSize().x * _currentFrame;
	rect.top = _spriteSheet->getSpriteSize().y * (_frameRow + (short)_spriteSheet->getDirection());
	rect.width = _spriteSheet->getSpriteSize().x;
	rect.height = _spriteSheet->getSpriteSize().y;

	_spriteSheet->cropSprite(rect);
}