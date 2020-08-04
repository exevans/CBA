#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <unordered_map>

#include "Direction.h"
//#include <LuaBridge.h>

extern "C"
{
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

class TextureManager;
class Animation;

using AnimationContainer = std::unordered_map<std::string, Animation*>;

class SpriteSheet {
public:
	SpriteSheet(TextureManager* texManager = nullptr);
	~SpriteSheet();

	bool loadSheet(const std::string&);
	void releaseSheet();

	void cropSprite(sf::IntRect&);
	void setSpritePosition(sf::Vector2f&);
	sf::Vector2f getSpritePosition();
	void setSpriteScale(sf::Vector2f&);

	void setDirection(const Direction&);
	const Direction& getDirection() const;
	bool setAnimation(const std::string&, const bool, const bool);
	std::string getCurrentAnimation();
	sf::Vector2i getSpriteSize();
	sf::Vector2i getTextureSize();

	void update(const float);
	void draw(sf::RenderWindow*);

private:
	std::string _texture;
	sf::Sprite _sprite;
	sf::Vector2i _spriteSize;
	sf::Vector2f _spriteScale;
	Direction _direction;
	std::string _animType;
	AnimationContainer _animations;
	Animation* _currentAnimation;
};

#endif