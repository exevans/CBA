#ifndef STATE_DIALOGUE_H
#define STATE_DIALOGUE_H

#include "State.h"
#include "pugixml.hpp"

class StateManager;
class EventDetails;

struct DialogueBox {
	sf::RectangleShape _background;
	sf::Font _font;
	sf::Text _text;
};

struct PlayerResponseBox {
	sf::RectangleShape _background;
	sf::Font _font;
	std::vector<sf::Text> _responses;	//different text options
};

class State_Dialogue : public State {
public:
	State_Dialogue(StateManager* stateManager) : State(stateManager) {};

	void onCreate() override;
	void onDestroy() override;

	void activate() override;
	void deactivate() override;

	void update(float) override;
	void draw() override;
private:
	void startDialogue();
	void processResponses();
	void updateDialogue(EventDetails*);
	void changeSelectedResponse(EventDetails*);

	pugi::xml_document doc;
	pugi::xml_node convoRoot;
	pugi::xml_node responseRoot;

	PlayerResponseBox _playerResponses;
	DialogueBox _characterDialogue;
	sf::RectangleShape _dimBackground;

	unsigned int _activeConversationID = 0;	//mainly for player use
	unsigned int _selectedResponseNum = 0;	//inputted response num

	unsigned int _highlightedResponseNum = 0;	//inputted response num
	bool _responseActivated = false; //sets response as answer
};

#endif