#include "TextureManager.h"

std::map<std::string, sf::Texture> TextureManager::_textures;

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
	for (auto it : _textures)
		delete &it;

	_textures.clear();
}

bool TextureManager::addTexture(std::string& filePath)
{
	if (_textures.find(filePath) == _textures.end())
	{
		sf::Texture newTexture;
		if (newTexture.loadFromFile(filePath))
		{
			_textures[filePath] = newTexture;
			std::cout << "new Texture added to tex manager" << std::endl;
		}
		else
		{
			return false;
			std::cout << "Could not add texture " << filePath << std::endl;
		}
	}
	return true;
}

sf::Texture& TextureManager::getTextureRef(std::string& texturePath)
{
	return _textures.find(texturePath)->second;
}