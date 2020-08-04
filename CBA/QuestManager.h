#ifndef QUESTMANAGER_H
#define QUESTMANAGER_H

#include "pugixml.hpp"
#include <SFML/Graphics.hpp>
#include "Observer.h"

class Entity;

class QuestManager : public Observer
{
public:
	QuestManager();
	static bool IsQuestActivated(std::string);
	static bool IsQuestStageReached(std::string, int);
	static bool isQuestFinished(std::string);

	void saveData();
	void loadData();
	virtual void onNotify(Entity*, std::string, unsigned int) override;
	virtual void onNotify(Entity*, std::string, unsigned int, unsigned int) override;
	void draw(sf::RenderWindow &);

private:
	void activateQuest(unsigned int);
	void progressQuest(unsigned int, unsigned int);

	static pugi::xml_document _questDoc;
	pugi::xml_node _QuestRoot;

	sf::RectangleShape _questTextBox;
	sf::Text _questDescription;
	//sf::Font font;

	sf::Clock _clock;
	bool _showingQuest = false;
};

#endif