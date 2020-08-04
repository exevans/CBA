#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "pugixml.hpp"
#include "System.h"
#include "Subject.h"

struct QuestComponent;

class DialogueSystem : public System
{
public:
	DialogueSystem();
	void update(float) override;

private:
	void startDialogue(Entity*, unsigned int);
	bool isDialogueConditionsMet(Entity*, pugi::xml_node*);
	void processResponses(Entity*);
	void updateDialogue(Entity*);
	bool isEndOfConversation();
	void checkForQuests(QuestComponent*);
	void checkForActions(Entity*);

	pugi::xml_document doc;
	pugi::xml_node convoRoot;
	pugi::xml_node responseRoot;

	bool _inDialogue = false;

	std::vector<sf::Text> _responses;	//different text options
	sf::RectangleShape _dialogTextBox;
	sf::Text _dialogText;
	sf::RectangleShape _dialogTextBoxP;
	sf::Font font;
};

#endif