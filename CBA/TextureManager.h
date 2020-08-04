#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	static bool addTexture(std::string&);
	static sf::Texture& getTextureRef(std::string&);

private:
	static std::map<std::string, sf::Texture> _textures;
};

#endif