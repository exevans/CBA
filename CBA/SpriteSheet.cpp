#include "SpriteSheet.h"
#include "TextureManager.h"
#include "Animation.h"
#include "Anim_Dir.h"

#include <sstream>
#include <fstream>

SpriteSheet::SpriteSheet(TextureManager* texManager) :
	_currentAnimation(nullptr),
	_spriteScale(1.f, 1.f),
	_direction(Direction::Down)
{

}

SpriteSheet::~SpriteSheet() {
	releaseSheet();
}

bool SpriteSheet::loadSheet(const std::string& file) {
	std::ifstream inFile;
	inFile.open(file);
	if (inFile.is_open()) {
		releaseSheet();
		std::string line;
		while (std::getline(inFile, line)) {
			if (line[0] == '/') { continue; }
			std::stringstream ss(line);
			std::string type;
			ss >> type;

			if (type == "Texture") {
				std::string texture;
				ss >> texture;
				if (!TextureManager::addTexture(texture)) {
					std::cerr << "Could not set up texture: " << texture << std::endl;
					continue;
				}
				_texture = texture;
				_sprite.setTexture(TextureManager::getTextureRef(_texture));
			}
			else if (type == "Size") {
				ss >> _spriteSize.x >> _spriteSize.y;
			}
			else if (type == "Scale") {
				ss >> _spriteScale.x >> _spriteScale.y;
				setSpriteScale(_spriteScale);
			} 
			else if (type == "AnimationType") {
				ss >> _animType;
			}
			else if (type == "Animation") {
				std::string name;
				ss >> name;
				if (_animations.find(name) != _animations.end()) { 
					std::cerr << "Duplicate animation: " << name << std::endl;
					continue; 
				}

				Animation* anim = nullptr;
				if (_animType == "Directional") {
					anim = new Anim_Dir();
				}
				else {
					std::cerr << "unknown animation type: " << _animType << std::endl;
					continue;
				}

				ss >> *anim;
				anim->setSpriteSheet(this);
				anim->_name = name;
				anim->reset();
				_animations.emplace(name, anim);

				if (_currentAnimation) { continue; }
				_currentAnimation = anim;
				_currentAnimation->play();
			}
		}

		_sprite.setOrigin(_sprite.getPosition().x + _sprite.getGlobalBounds().width / 2, _sprite.getPosition().y + _sprite.getGlobalBounds().height);	//set origin to centre of texture
		_sprite.setOrigin(_sprite.getOrigin().x / _sprite.getScale().x, _sprite.getOrigin().y / _sprite.getScale().y);

		inFile.close();
		return true;
	}
	else {
		std::cerr << "Could not load sprite sheet: " << file << std::endl;
		return false;
	}
}

void SpriteSheet::releaseSheet() {
	_currentAnimation = nullptr;
	while (_animations.begin() != _animations.end()) {
		delete _animations.begin()->second;
		_animations.erase(_animations.begin());
	}
}

void SpriteSheet::cropSprite(sf::IntRect& rect) {
	_sprite.setTextureRect(rect);
}

void SpriteSheet::setSpritePosition(sf::Vector2f& pos) {
	_sprite.setPosition(pos);
}

sf::Vector2f SpriteSheet::getSpritePosition() {
	return _sprite.getPosition();
}

void SpriteSheet::setSpriteScale(sf::Vector2f& scale) {
	_sprite.setScale(scale);
}

void SpriteSheet::setDirection(const Direction& dir) {
	if (dir == _direction) { return; }
	_direction = dir;
	_currentAnimation->cropSprite();
}

const Direction& SpriteSheet::getDirection() const {
	return _direction;
}

bool SpriteSheet::setAnimation(const std::string& aniName, const bool looping, const bool play) {
	auto itr = _animations.find(aniName);
	if (itr == _animations.end()) { return false; }
	if (itr->second == _currentAnimation) { return false; }
	if (_currentAnimation) { _currentAnimation->stop(); }
	_currentAnimation = itr->second;
	_currentAnimation->setLooping(looping);
	if (play) {
		_currentAnimation->play();
	}
	_currentAnimation->cropSprite();
	return true;
}

std::string SpriteSheet::getCurrentAnimation() {
	return "";
}

sf::Vector2i SpriteSheet::getSpriteSize() {
	return _spriteSize;
}

sf::Vector2i SpriteSheet::getTextureSize() {
	return sf::Vector2i(TextureManager::getTextureRef(_texture).getSize());
}

void SpriteSheet::update(const float deltaTime) {
	if (_currentAnimation) {
		_currentAnimation->update(deltaTime);
	}
}

void SpriteSheet::draw(sf::RenderWindow* win) {
	win->draw(_sprite);
}