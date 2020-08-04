/*#ifndef DIALOGUEMANAGER_H
#define DIALOGUEMANAGER_H

#include <SFML/Graphics.hpp>
#include "pugixml.hpp"
#include <memory>
#include "Subject.h"
#include "Window.h"

class Entity;
struct QuestComponent;

class DialogueManager : public Subject
{
public:
	DialogueManager(Window*);
	void resetGraphics();
	void update(float);
	void draw(sf::RenderWindow&);

	static Entity* _Player;
	static Entity* _otherSpeaker;
	static bool _newConversation;
	static bool _inDialogue;

	//Subject _questUpdated;
private:
	void handleEvents();
	void startDialogue();
	bool isDialogueConditionsMet(Entity*, pugi::xml_node*);
	void processResponses(Entity*);
	void updateDialogue(Entity*);
	bool isEndOfConversation();
	void checkForQuests(QuestComponent*);
	void checkForActions(Entity*);

	Window* _window;	//reference to main window

	pugi::xml_document doc;
	pugi::xml_node convoRoot;
	pugi::xml_node responseRoot;

	std::vector<sf::Text> _responses;	//different text options
	sf::RectangleShape _dialogTextBox;
	sf::Text _dialogText;
	sf::RectangleShape _dialogTextBoxP;
	//sf::Font font;

	unsigned int _activeConversationID = 0;	//mainly for player use
	unsigned int _selectedResponseNum = 0;	//inputted response num

	unsigned int _highlightedResponseNum = 0;	//inputted response num
	bool _responseActivated = false; //sets response as answer
};

#endif*/