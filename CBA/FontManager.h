#ifndef FONT_MANG_H
#define FONT_MANG_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

class FontManager
{
public:
	FontManager();
	~FontManager();
	static void addFont(std::string&);
	static sf::Font& getFontRef(std::string&);

private:
	static std::map<std::string, sf::Font> _fonts;
};

#endif