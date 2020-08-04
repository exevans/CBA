#include "TextManager.h"
#include "Window.h"
#include "FontManager.h"

TextManager::TextManager(EntityManager* entMang) : _entityManager(entMang) {

}

TextManager::~TextManager() {
	std::cout << "started to destryot text manager" << std::endl;
	purge();
}

void TextManager::addTextMessage(const EntityID& ent, const std::string& text, float timeLimit, sf::Vector2f position) {
	TextData textData;
	//setup text appearence
	textData._text.setCharacterSize(25);
	textData._text.setFillColor(sf::Color::White);

	FontManager::addFont(std::string("Assets/Fonts/BERNHC.ttf"));
	textData._text.setFont(FontManager::getFontRef(std::string("Assets/Fonts/BERNHC.ttf")));

	textData._text.setString(text);
	textData._text.setPosition(position);
	textData._timeLimit = timeLimit;

	_textContainer[ent] = textData;
}

void TextManager::removeTextMessage(const EntityID& ent) {
	auto itr = _textContainer.find(ent);
	if (itr == _textContainer.end()) { return; }

	_textContainer.erase(itr);
}

void TextManager::purge() {
	_textContainer.clear();
}

void TextManager::update(const float delta) {
	//update time
	for (auto& it : _textContainer) {
		//if perm keep
		if (it.second._timeLimit == 0) { continue; }

		//check if over time limit
		it.second._timePassed += delta;
		if (it.second._timePassed > it.second._timeLimit) {
			it.second._valid = false;
		}
	}
	//remove invalid textobjects
	for (auto& itr = _textContainer.begin(); itr != _textContainer.end(); ) {
		if (!itr->second._valid) {
			itr = _textContainer.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void TextManager::draw(Window* win) {
	for (auto& it : _textContainer) {
		win->draw(it.second._text);
	}
}