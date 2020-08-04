#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include "EntityManager.h"

class Window;

struct TextData {
	sf::Text _text;
	float _timePassed = 0;
	bool _valid = true;
	float _timeLimit;
};

using EntityID = unsigned int;
using TextContainer = std::unordered_map<EntityID, TextData>;

class TextManager {
public:
	TextManager(EntityManager*);
	~TextManager();
	void addTextMessage(const EntityID&, const std::string&, float, sf::Vector2f);
	void removeTextMessage(const EntityID&);
	void purge();
	void update(const float);
	void draw(Window*);

private:
	TextContainer _textContainer;
	EntityManager* _entityManager;
};

#endif