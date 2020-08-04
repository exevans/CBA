/*#include "DialogueManager.h"
#include <iostream>
#include "Entity.h"
#include "KeyBinding.h"
#include "FontManager.h"
#include "QuestManager.h"

bool DialogueManager::_newConversation = false;
bool DialogueManager::_inDialogue = false;
Entity* DialogueManager::_otherSpeaker;
Entity* DialogueManager::_Player;

DialogueManager::DialogueManager(Window* win)
	:_window(win)
{
	_dialogTextBox.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, 200.f));
	_dialogTextBox.setFillColor(sf::Color::Black);
	_dialogTextBox.setOutlineColor(sf::Color::White);
	_dialogTextBox.setOutlineThickness(2.f);
	_dialogTextBox.setPosition(0.f, 0.f);

	_dialogText.setCharacterSize(22);
	_dialogText.setFillColor(sf::Color::White);
	_dialogText.setPosition(_dialogTextBox.getPosition().x + 10.f, _dialogTextBox.getPosition().y + 10.f);

	FontManager::addFont(std::string("Assets/Fonts/BERNHC.ttf"));
	//font.loadFromFile("Assets/Fonts/BERNHC.ttf");
	_dialogText.setFont(FontManager::getFontRef(std::string("Assets/Fonts/BERNHC.ttf")));
	_dialogText.setString("TEXT");

	//num2
	_dialogTextBoxP.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, 200.f));
	_dialogTextBoxP.setFillColor(sf::Color::Black);
	_dialogTextBoxP.setOutlineColor(sf::Color::White);
	_dialogTextBoxP.setOutlineThickness(2.f);
	_dialogTextBoxP.setPosition(0.f, sf::VideoMode::getDesktopMode().height - _dialogTextBoxP.getGlobalBounds().height);
	pugi::xml_parse_result result = doc.load_file("Assets/Data/Dialogue/Dialogue_0.xml");
}

void DialogueManager::resetGraphics()
{
	_dialogTextBoxP.setPosition(0.f, _window->getWinSize().y - _dialogTextBoxP.getGlobalBounds().height);
	//_dialogText.setPosition(_dialogTextBox.getPosition().x + 10.f, _dialogTextBox.getPosition().y + 40.f);
}

void DialogueManager::update(float deltaTime)
{
	if (this->_newConversation)
	{
		this->startDialogue();
		_newConversation = false;
		_inDialogue = true;
	}
	else if (this->_inDialogue)
	{
		//TEMP
		this->handleEvents();

		//if response highlighted and selected
		if (this->_highlightedResponseNum > 0 && this->_responseActivated)
		{
			_selectedResponseNum = _highlightedResponseNum;
			_responseActivated = false;
		}
		else if (_highlightedResponseNum > 0 && _highlightedResponseNum >= _responses.size())	//set to maximum
		{
			_highlightedResponseNum = _responses.size();
		}
		else if (_highlightedResponseNum <= 1 )	//set to minimum
		{
			_highlightedResponseNum = 1;
		}

		//check if mouse intersects option
		for (size_t i = 0; i < _responses.size(); ++i)
		{
			if (_responses[i].getGlobalBounds().contains((_window->getMouseWorldPos())))
				_highlightedResponseNum = i + 1;
		}

		if (_highlightedResponseNum > 0 && _highlightedResponseNum <= _responses.size())	//highlight decision
		{
			for (std::vector<sf::Text>::iterator i = _responses.begin(); i != _responses.end(); ++i)
			{
				i->setOutlineThickness(0);
			}

			//set highlighted response
			_responses[_highlightedResponseNum - 1].setOutlineColor(sf::Color::Red);
			_responses[_highlightedResponseNum - 1].setOutlineThickness(1.f);
		}

		if (_selectedResponseNum > 0)
		{
			_highlightedResponseNum = 0;

			if (!this->isEndOfConversation())
				this->updateDialogue(_Player);
			else
			{
				this->_inDialogue = false;
			}
		}
	}
}

void DialogueManager::handleEvents()
{
	_selectedResponseNum = 0;

	if (KeyBinder::isEventPending("Option1"))
		this->_selectedResponseNum = 1;
	else if (KeyBinder::isEventPending("Option2"))
		this->_selectedResponseNum = 2;
	else if (KeyBinder::isEventPending("Option3"))
		this->_selectedResponseNum = 3;
	else if (KeyBinder::isEventPending("Option4"))
		this->_selectedResponseNum = 4;

	if (KeyBinder::isEventPending("NextItem"))
	{
		_highlightedResponseNum++;
		std::cout << "Current highlighted: " <<_highlightedResponseNum << std::endl;
	}
	else if (KeyBinder::isEventPending("PreviousItem"))
	{
		_highlightedResponseNum--;
		std::cout << "Current highlighted: " << _highlightedResponseNum << std::endl;
	}

	if (KeyBinder::isEventPending("Interact") || KeyBinder::isEventPending("Activate"))
		_responseActivated = true;
}

void DialogueManager::draw(sf::RenderWindow& win)
{
	win.setView(win.getDefaultView());	//Sets view to default

	if (this->_inDialogue)
	{
		win.draw(_dialogTextBox);
		win.draw(_dialogText);

		win.draw(_dialogTextBoxP);
		//win.draw(_dialogTextP);

		for (std::vector<sf::Text>::iterator i = _responses.begin(); i != _responses.end(); ++i)
		{
			win.draw(*i);
		}
	}
}

void DialogueManager::startDialogue()
{
	std::cout << "Converstation started: " << std::endl;
	
	unsigned int baseConvoID = _otherSpeaker->get<DialogueComponent>()->_baseConversationID;
	std::string charNumS = std::to_string(baseConvoID);

	convoRoot = doc.child("Conversations").find_child_by_attribute("Conversation", "ID", charNumS.c_str()).child("DialogEntries");

	for (pugi::xml_node i = convoRoot.first_child(); i; i = i.next_sibling())
		if (i.attribute("IsRoot").as_bool() == true)
		{
			convoRoot = i;
			if (this->isDialogueConditionsMet(_Player, &convoRoot))
				break;
		}

	std::cout << convoRoot.first_child().first_child().child("DialogueText").text().as_string() << std::endl;
	this->_dialogText.setString(_otherSpeaker->getName() + ":\n\n" + convoRoot.first_child().first_child().child("DialogueText").text().as_string());//set gui text
	this->processResponses(_Player);
}

bool DialogueManager::isDialogueConditionsMet(Entity* entity, pugi::xml_node* inputRoot)
{
	if (inputRoot->attribute("Conditional").as_bool() == true)
	{
		//Quest stage reached conditions
		pugi::xml_node conditionRoot = inputRoot->child("Fields").find_child_by_attribute("Field", "Type", "Conditions").find_child_by_attribute("Condition", "ConditionType", "QuestReached");
		for (pugi::xml_node i = conditionRoot.first_child(); i; i = i.next_sibling())
		{
			//if (!entity->get<QuestComponent>()->IsQuestActivated(i))
			if (!QuestManager::IsQuestActivated(i.attribute("ID").as_string()))
				return false;

			//if (!entity->get<QuestComponent>()->IsQuestStageReached(i))
			if (!QuestManager::IsQuestStageReached(i.attribute("ID").as_string(), i.attribute("Stage").as_uint()))
				return false;
		}

		//Quest Stage Not reached
		conditionRoot = inputRoot->child("Fields").find_child_by_attribute("Field", "Type", "Conditions").find_child_by_attribute("Condition", "ConditionType", "QuestNotReached");
		for (pugi::xml_node i = conditionRoot.first_child(); i; i = i.next_sibling())
		{
			//if (entity->get<QuestComponent>()->IsQuestStageReached(i))
			if (QuestManager::IsQuestStageReached(i.attribute("ID").as_string(), i.attribute("Stage").as_uint()))
				return false;
		}

		//Quest Not Started conditions
		conditionRoot = inputRoot->child("Fields").find_child_by_attribute("Field", "Type", "Conditions").find_child_by_attribute("Condition", "ConditionType", "QuestNotStarted");
		for (pugi::xml_node i = conditionRoot.first_child(); i; i = i.next_sibling())
		{
			//if (entity->get<QuestComponent>()->IsQuestActivated(i))
			if (QuestManager::IsQuestActivated(i.attribute("ID").as_string()))
				return false;
		}

		conditionRoot = inputRoot->child("Fields").find_child_by_attribute("Field", "Type", "Conditions").find_child_by_attribute("Condition", "ConditionType", "QuestFinished");
		for (pugi::xml_node i = conditionRoot.first_child(); i; i = i.next_sibling())
		{
			//if (!entity->get<QuestComponent>()->isQuestFinished(i))
			if (!QuestManager::isQuestFinished(i.attribute("ID").as_string()))
				return false;
		}

		conditionRoot = inputRoot->child("Fields").find_child_by_attribute("Field", "Type", "Conditions").find_child_by_attribute("Condition", "ConditionType", "QuestNotFinished");
		for (pugi::xml_node i = conditionRoot.first_child(); i; i = i.next_sibling())
		{
			//if (entity->get<QuestComponent>()->isQuestFinished(i))
			if (QuestManager::isQuestFinished(i.attribute("ID").as_string()))
				return false;
		}

		conditionRoot = inputRoot->child("Fields").find_child_by_attribute("Field", "Type", "Conditions").find_child_by_attribute("Condition", "ConditionType", "ItemInInventory");
		for (pugi::xml_node i = conditionRoot.first_child(); i; i = i.next_sibling())
		{
			if (!entity->get<InventoryComponent>()->isItemInInventory(i.attribute("ID").as_string()))
				return false;
		}
	}
	return true;
}

void DialogueManager::processResponses(Entity* entity)
{
	std::string newL = "\n";
	unsigned int x = 0;

	responseRoot = convoRoot.first_child().first_child().child("Responses");
	_responses.clear();

	for (pugi::xml_node i = responseRoot.first_child(); i; i = i.next_sibling())
	{
		if (isDialogueConditionsMet(entity, &i))
		{
			//TEMP
			sf::Text response;
			response.setCharacterSize(22);
			response.setFillColor(sf::Color::White);
			response.setFont(FontManager::getFontRef(std::string("Assets/Fonts/BERNHC.ttf")));
			response.setString(i.text().as_string());
			response.setPosition(_dialogTextBoxP.getPosition().x + 10.f, _dialogTextBoxP.getPosition().y + (x * response.getGlobalBounds().height) + (4 * x) + 10);

			_responses.push_back(response);
			x++;
		}
	}
}

void DialogueManager::updateDialogue(Entity* entity)
{
	unsigned int responseNum = _selectedResponseNum;

	unsigned int responses = 1;

	for (pugi::xml_node i = responseRoot.first_child(); i; i = i.next_sibling())
		if (isDialogueConditionsMet(entity, &i))
		{
			if (responseNum == responses)
			{
				convoRoot = convoRoot.parent().find_child_by_attribute("DialogEntry", "ID", i.attribute("DestinationDialogID").value());
				std::cout << convoRoot.first_child().first_child().child("DialogueText").text().as_string() << std::endl;
				this->_dialogText.setString(_otherSpeaker->getName() + ":\n\n" + convoRoot.first_child().first_child().child("DialogueText").text().as_string());//set gui text

				checkForActions(entity);
				checkForQuests(entity->get<QuestComponent>());
				processResponses(entity);
			}
			responses++;
		}
}

bool DialogueManager::isEndOfConversation()
{
	if (convoRoot.attribute("IsEnd").as_bool() == true)
		return true;

	return false;
}

void DialogueManager::checkForQuests(QuestComponent* entQuestComp)
{
	if (convoRoot.attribute("QuestActivator").as_bool() == true)
	{
		entQuestComp->_convoRoot = convoRoot; // temp for observer
		this->notify(entQuestComp->getOwnerEntity(), "EVENT_QUEST_ACTIVATED", entQuestComp->_convoRoot.child("OutgoingLinks").child("Link").attribute("ActivatedQuestID").as_uint());
	}
	else if (convoRoot.attribute("QuestUpdater").as_bool() == true)
	{
		entQuestComp->_convoRoot = convoRoot; // temp for observer
		this->notify(entQuestComp->getOwnerEntity(), "EVENT_QUEST_UPDATED", entQuestComp->_convoRoot.child("Fields").find_child_by_attribute("Field", "Type", "Quest").child("NextQuestID").text().as_uint(), entQuestComp->_convoRoot.child("Fields").find_child_by_attribute("Field", "Type", "Quest").child("NextQuestStage").text().as_uint());
	}
}

void DialogueManager::checkForActions(Entity* entity)
{
	if (convoRoot.attribute("ActionRequired").as_bool() == true)
	{
		for (pugi::xml_node i = convoRoot.child("Fields").find_child_by_attribute("Field", "Type", "Action").first_child(); i; i = i.next_sibling())
		{
			std::cout << "TESITNG: " << i.attribute("ActionType").as_string() << std::endl;
			std::string action = i.attribute("ActionType").as_string();

			if (action == "RemoveItemFromInventory")
			{
				entity->get<InventoryComponent>()->removeEntity(i.attribute("ItemTypeID").as_string());
				continue;
			}
			else if (action == "AddItemToInventory")
			{
				//Entity* newItem = new Entity;
				std::shared_ptr<Entity> newItem(new Entity());
				//newItem->loadScript(GameManager::L, "Assets/Scripts/Items.lua", i.attribute("ItemTypeID").as_string());

				entity->get<InventoryComponent>()->addEntity(std::move(newItem));
				continue;
			}
		}
	}
}*/