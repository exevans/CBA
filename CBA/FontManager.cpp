#include "FontManager.h"

std::map<std::string, sf::Font> FontManager::_fonts;

FontManager::FontManager()
{

}

FontManager::~FontManager()
{
	for (auto it : _fonts)
		delete &it;

	_fonts.clear();
}

void FontManager::addFont(std::string& filePath)
{
	if (_fonts.find(filePath) == _fonts.end())
	{
		sf::Font newFont;
		if (newFont.loadFromFile(filePath))
		{
			_fonts[filePath] = newFont;
			std::cout << "new Texture added to tex manager" << std::endl;
		}
		else
		{
			std::cout << "Could not add texture " << filePath << std::endl;
		}
	}
}

sf::Font& FontManager::getFontRef(std::string& texturePath)
{
	return _fonts.find(texturePath)->second;
}