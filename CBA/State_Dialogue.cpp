#include <sstream>
#include <regex>

#include "State_Dialogue.h"
#include "StateManager.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "Component.h"
#include "Window.h"

void State_Dialogue::onCreate() {
	setTransparent(true);

	pugi::xml_parse_result result = doc.load_file("Assets/Data/Dialogue/Dialogue_0.xml");

	sf::Vector2u winSize = _stateManager->getShared()->_window->getWinSize();

	EventManager* evManag = _stateManager->getShared()->_eventManager;
	//evManag->addCallBack(StateType::Dialogue, "Mouse_Left", &State_Dialogue::updateDialogue, this);
	evManag->addCallBack(StateType::Dialogue, "Player_Interact", &State_Dialogue::updateDialogue, this);
	evManag->addCallBack(StateType::Dialogue, "Player_MoveDown", &State_Dialogue::changeSelectedResponse, this);
	evManag->addCallBack(StateType::Dialogue, "Player_MoveUp", &State_Dialogue::changeSelectedResponse, this);

	_dimBackground.setSize(sf::Vector2f(winSize));
	_dimBackground.setFillColor(sf::Color(0, 0, 0, 150));

	if (!_characterDialogue._font.loadFromFile("Assets/Fonts/BernHC.ttf")) {
		std::cout << "Could not load font from file!" << std::endl;
	}
	if (!_playerResponses._font.loadFromFile("Assets/Fonts/BernHC.ttf")) {
		std::cout << "Could not load font from file!" << std::endl;
	}

	_characterDialogue._background.setSize(sf::Vector2f(winSize.x - winSize.x / 4, winSize.y / 5));
	_characterDialogue._background.setOrigin(_characterDialogue._background.getGlobalBounds().width / 2, _characterDialogue._background.getGlobalBounds().height / 2);
	_characterDialogue._background.setPosition(winSize.x / 2, _characterDialogue._background.getGlobalBounds().height / 2 + 20);
	_characterDialogue._background.setFillColor(sf::Color(44, 53, 68, 255));
	_characterDialogue._background.setOutlineColor(sf::Color::Black);
	_characterDialogue._background.setOutlineThickness(2.0);

	_characterDialogue._text.setFont(_characterDialogue._font);
	_characterDialogue._text.setString("Testing");
	_characterDialogue._text.setPosition(_characterDialogue._background.getPosition().x - _characterDialogue._background.getGlobalBounds().width / 2 + 10, _characterDialogue._background.getPosition().y - _characterDialogue._background.getGlobalBounds().height / 2 + 10);

	
	//setup response window
	_playerResponses._background.setSize(sf::Vector2f(winSize.x - winSize.x / 4, winSize.y / 5));
	_playerResponses._background.setOrigin(_playerResponses._background.getGlobalBounds().width / 2, _playerResponses._background.getGlobalBounds().height / 2);
	_playerResponses._background.setPosition(winSize.x / 2, winSize.y - _playerResponses._background.getGlobalBounds().height / 2 - 20);
	_playerResponses._background.setFillColor(sf::Color(44, 53, 68, 255));
	_playerResponses._background.setOutlineColor(sf::Color::Black);
	_playerResponses._background.setOutlineThickness(2.0);
}

void State_Dialogue::onDestroy() {
	//_stateManager->getShared()->_eventManager->removeCallback(StateType::Dialogue, "Mouse_Left");
	_stateManager->getShared()->_eventManager->removeCallback(StateType::Dialogue, "Player_Interact");
	_stateManager->getShared()->_eventManager->removeCallback(StateType::Dialogue, "Player_MoveUp");
	_stateManager->getShared()->_eventManager->removeCallback(StateType::Dialogue, "Player_MoveDown");
}

void State_Dialogue::update(float deltaTime) {
	if (_highlightedResponseNum > 0 && _highlightedResponseNum >= _playerResponses._responses.size())	//set to maximum
	{
		_highlightedResponseNum = _playerResponses._responses.size();
	}
	else if (_highlightedResponseNum <= 1)	//set to minimum
	{
		_highlightedResponseNum = 1;
	}

	//check if mouse intersects option
	for (size_t i = 0; i < _playerResponses._responses.size(); ++i)
	{
		if (_playerResponses._responses[i].getGlobalBounds().contains(_stateManager->getShared()->_window->getMouseWorldPos()))
			_highlightedResponseNum = i + 1;
	}

	if (_highlightedResponseNum > 0 && _highlightedResponseNum <= _playerResponses._responses.size())	//highlight decision
	{
		for (std::vector<sf::Text>::iterator i = _playerResponses._responses.begin(); i != _playerResponses._responses.end(); ++i)
		{
			i->setOutlineThickness(0);
		}

		//set highlighted response
		_playerResponses._responses[_highlightedResponseNum - 1].setOutlineColor(sf::Color::Red);
		_playerResponses._responses[_highlightedResponseNum - 1].setOutlineThickness(1.f);
	}
}

void State_Dialogue::draw() {
	_stateManager->getShared()->_window->draw(_dimBackground);

	_stateManager->getShared()->_window->draw(_characterDialogue._background);
	_stateManager->getShared()->_window->draw(_characterDialogue._text);

	_stateManager->getShared()->_window->draw(_playerResponses._background);
	//draw responses
	for (auto& response : _playerResponses._responses) {
		_stateManager->getShared()->_window->draw(response);
	}
}

void State_Dialogue::activate() {
	//setup inital convosation details
	startDialogue();
}

void State_Dialogue::deactivate() {}

void State_Dialogue::startDialogue() {
	std::cout << "Converstation started: " << std::endl;

	DialogueComponent* playerDialogue = _stateManager->getShared()->_entityManager->getComponent<DialogueComponent>(0, ComponentType::Dialogue);
	DialogueComponent* charDialogue = _stateManager->getShared()->_entityManager->getComponent<DialogueComponent>(playerDialogue->_otherEntity, ComponentType::Dialogue);

	unsigned int baseConvoID = charDialogue->_baseConversationID;
	std::string charNumS = std::to_string(baseConvoID);

	convoRoot = doc.child("Conversations").find_child_by_attribute("Conversation", "ID", charNumS.c_str()).child("DialogEntries");

	for (pugi::xml_node i = convoRoot.first_child(); i; i = i.next_sibling())
		if (i.attribute("IsRoot").as_bool() == true)
		{
			convoRoot = i;
			//if (this->isDialogueConditionsMet(_Player, &convoRoot))
				break;
		}

	std::cout << convoRoot.first_child().first_child().child("DialogueText").text().as_string() << std::endl;
	this->_characterDialogue._text.setString(_stateManager->getShared()->_entityManager->getEntityInfo(playerDialogue->_otherEntity).name + ":\n\n" + convoRoot.first_child().first_child().child("DialogueText").text().as_string());//set gui text
	this->processResponses(/*_Player*/);
}

void State_Dialogue::processResponses() {
	std::string newL = "\n";
	unsigned int x = 0;

	responseRoot = convoRoot.first_child().first_child().child("Responses");
	_playerResponses._responses.clear();

	for (pugi::xml_node i = responseRoot.first_child(); i; i = i.next_sibling())
	{
		//if (isDialogueConditionsMet(entity, &i))
		{
			//TEMP
			sf::Text response;
			response.setCharacterSize(22);
			response.setFillColor(sf::Color::White);
			response.setFont(_playerResponses._font);
			response.setString(i.text().as_string());
			response.setPosition(_playerResponses._background.getPosition().x - _playerResponses._background.getGlobalBounds().width/2 + 10.f, _playerResponses._background.getPosition().y + (x * response.getGlobalBounds().height) + (4 * x) + 10);

			_playerResponses._responses.push_back(response);
			x++;
		}
	}
}

void State_Dialogue::updateDialogue(EventDetails* details) {
	//check if an ending action
	if (convoRoot.attribute("IsEnd").as_bool() == true) {
		_stateManager->getShared()->_entityManager->getComponent<DialogueComponent>(0, ComponentType::Dialogue)->_inConversation = false;
		_stateManager->requestSwitchTo(StateType::Game);
		return;
	}

	_selectedResponseNum = _highlightedResponseNum;
	if (_selectedResponseNum <= 0) { return; }
	_highlightedResponseNum = 0;

	unsigned int responseNum = _selectedResponseNum;
	unsigned int responses = 1;

	for (pugi::xml_node i = responseRoot.first_child(); i; i = i.next_sibling())
		//if (isDialogueConditionsMet(entity, &i))
		{
			if (responseNum == responses)
			{
				convoRoot = convoRoot.parent().find_child_by_attribute("DialogEntry", "ID", i.attribute("DestinationDialogID").value());
				std::cout << convoRoot.first_child().first_child().child("DialogueText").text().as_string() << std::endl;
				_characterDialogue._text.setString(convoRoot.first_child().first_child().child("DialogueText").text().as_string());//set gui text
				
				//sort length of text boxes
				if (_characterDialogue._text.getGlobalBounds().width > _characterDialogue._background.getGlobalBounds().width - 20) {
					std::string text = _characterDialogue._text.getString();
					text.insert(60, "\n");
					_characterDialogue._text.setString(text);
				}

				//checkForActions(entity);
				//checkForQuests(entity->get<QuestComponent>());
				processResponses();
				return;
			}
			responses++;
		}
}

void State_Dialogue::changeSelectedResponse(EventDetails* details) {
	if (details->_name == "Player_MoveDown") {
		_highlightedResponseNum++;
	}
	else if (details->_name == "Player_MoveUp") {
		_highlightedResponseNum--;
	}
}