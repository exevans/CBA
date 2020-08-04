#include "DialogueSystem.h"
#include "Component.h"
#include "Entity.h"

DialogueSystem::DialogueSystem()
{
	std::bitset<32> _systemLock(std::string("100000000000"));
	_systemLockID = _systemLock.to_ulong();
	_systemType = systems::Dialogue;

	std::map<unsigned int, Component*> test;
	_components[std::type_index(typeid(DialogueComponent))] = test;

	_dialogTextBox.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, 150.f));
	_dialogTextBox.setFillColor(sf::Color::Black);
	_dialogTextBox.setOutlineColor(sf::Color::White);
	_dialogTextBox.setOutlineThickness(2.f);
	_dialogTextBox.setPosition(0.f, 0.f);

	_dialogText.setCharacterSize(22);
	_dialogText.setFillColor(sf::Color::White);
	_dialogText.setPosition(_dialogTextBox.getPosition().x + 10.f, _dialogTextBox.getPosition().y + 40.f);

	font.loadFromFile("Assets/Fonts/BERNHC.ttf");
	_dialogText.setFont(font);
	_dialogText.setString("TEXT");

	//num2
	_dialogTextBoxP.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, 150.f));
	_dialogTextBoxP.setFillColor(sf::Color::Black);
	_dialogTextBoxP.setOutlineColor(sf::Color::White);
	_dialogTextBoxP.setOutlineThickness(2.f);
	_dialogTextBoxP.setPosition(0.f, 450.f);
}

void DialogueSystem::init(std::vector<Entity*> entities)
{

}

void DialogueSystem::update(float deltaTime)
{
	auto& dialogueComps = _components.find(std::type_index(typeid(DialogueComponent)))->second;
	for (std::map<unsigned int, Component*>::iterator it = dialogueComps.begin(); it != dialogueComps.end(); ++it)
	{
		auto dialogComp = this->get<DialogueComponent>(it->first);

		if (dialogComp->_newConversation)
		{
			this->startDialogue(it->second->getOwnerEntity(), dialogComp->_activeConversationID);
			dialogComp->_inDialogue = true;
			dialogComp->_newConversation = false;
			this->_inDialogue = true;
		}
		else if (dialogComp->_inDialogue)
		{
			//TEMP
			if (dialogComp->_highlightedResponseNum > 0 && dialogComp->_responseActivated)
			{
				dialogComp->_selectedResponseNum = dialogComp->_highlightedResponseNum;
				dialogComp->_responseActivated = false;
			}
			else if (dialogComp->_highlightedResponseNum > 0 && dialogComp->_highlightedResponseNum <= _responses.size())
			{
				for (std::vector<sf::Text>::iterator i = _responses.begin(); i != _responses.end(); ++i)
				{
					i->setOutlineThickness(0);
				}

				_responses[dialogComp->_highlightedResponseNum - 1].setOutlineColor(sf::Color::Red);
				_responses[dialogComp->_highlightedResponseNum - 1].setOutlineThickness(1.f);
			}
			else if (dialogComp->_highlightedResponseNum > 0 && dialogComp->_highlightedResponseNum >= _responses.size())
			{
				dialogComp->_highlightedResponseNum = _responses.size();
				std::cout << "Current highlighted: " << dialogComp->_highlightedResponseNum << std::endl;
			}
			//end of new

			if (dialogComp->_selectedResponseNum > 0)
			{
				dialogComp->_highlightedResponseNum = 0;

				if (!this->isEndOfConversation())
					this->updateDialogue(it->second->getOwnerEntity());
				else
				{
					dialogComp->_inDialogue = false;
					this->_inDialogue = false;

					//new temp
					dialogComp->_otherChar->get<DialogueComponent>()->_inDialogue = false;
				}
			}
		}
	}
}

void DialogueSystem::drawGui(sf::RenderWindow& win)
{
	if (this->_inDialogue)
	{
		win.draw(_dialogTextBox);
		win.draw(_dialogText);

		win.draw(_dialogTextBoxP);
		//win.draw(_dialogTextP);

		//TEMP
		for (std::vector<sf::Text>::iterator i = _responses.begin(); i != _responses.end(); ++i)
		{
			win.draw(*i);
		}
	}
}

void DialogueSystem::startDialogue(Entity* entity, unsigned int baseConvoID)
{
	std::cout << "Converstation started: " << std::endl;

	pugi::xml_parse_result result = doc.load_file("Assets/Data/Dialogue/Dialogue_0.xml");

	std::string charNumS = std::to_string(baseConvoID);
	
	convoRoot = doc.child("Conversations").find_child_by_attribute("Conversation", "ID", charNumS.c_str()).first_child();

	for (pugi::xml_node i = convoRoot.first_child(); i; i = i.next_sibling())
		if (i.attribute("IsRoot").as_bool() == true)
		{
			convoRoot = i;
			if (this->isDialogueConditionsMet(entity, &convoRoot))
				break;
		}

	std::cout << convoRoot.first_child().first_child().child("DialogueText").text().as_string() << std::endl;
	this->_dialogText.setString(convoRoot.first_child().first_child().child("DialogueText").text().as_string());//set gui text
	this->processResponses(entity);
}

bool DialogueSystem::isDialogueConditionsMet(Entity* entity, pugi::xml_node* inputRoot)
{
	if (inputRoot->attribute("Conditional").as_bool() == true)
	{
		//Quest stage reached conditions
		pugi::xml_node conditionRoot = inputRoot->child("Fields").find_child_by_attribute("Field", "Type", "Conditions").find_child_by_attribute("Condition", "ConditionType", "QuestReached");
		for (pugi::xml_node i = conditionRoot.first_child(); i; i = i.next_sibling())
		{
			if (!entity->get<QuestComponent>()->IsQuestActivated(i))
				return false;

			if (!entity->get<QuestComponent>()->IsQuestStageReached(i))
				return false;
		}
		//Quest Stage Not reached
		conditionRoot = inputRoot->child("Fields").find_child_by_attribute("Field", "Type", "Conditions").find_child_by_attribute("Condition", "ConditionType", "QuestNotReached");
		for (pugi::xml_node i = conditionRoot.first_child(); i; i = i.next_sibling())
		{
			if (entity->get<QuestComponent>()->IsQuestStageReached(i))
				return false;
		}

		//Quest Not Started conditions
		conditionRoot = inputRoot->child("Fields").find_child_by_attribute("Field", "Type", "Conditions").find_child_by_attribute("Condition", "ConditionType", "QuestNotStarted");
		for (pugi::xml_node i = conditionRoot.first_child(); i; i = i.next_sibling())
		{
			if (entity->get<QuestComponent>()->IsQuestActivated(i))
				return false;
		}

		conditionRoot = inputRoot->child("Fields").find_child_by_attribute("Field", "Type", "Conditions").find_child_by_attribute("Condition", "ConditionType", "QuestFinished");
		for (pugi::xml_node i = conditionRoot.first_child(); i; i = i.next_sibling())
		{	
			if (!entity->get<QuestComponent>()->isQuestFinished(i))
				return false;
		}

		conditionRoot = inputRoot->child("Fields").find_child_by_attribute("Field", "Type", "Conditions").find_child_by_attribute("Condition", "ConditionType", "QuestNotFinished");
		for (pugi::xml_node i = conditionRoot.first_child(); i; i = i.next_sibling())
		{
			if (entity->get<QuestComponent>()->isQuestFinished(i))
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

void DialogueSystem::processResponses(Entity* entity)
{
	std::string tempResponses;
	std::string newL = "\n";
	unsigned int x = 0;

	responseRoot = convoRoot.first_child().first_child().child("Responses");
	_responses.clear();

	for (pugi::xml_node i = responseRoot.first_child(); i; i = i.next_sibling())
	{
		if (isDialogueConditionsMet(entity ,&i))
		{
			x++;
			tempResponses += (std::to_string(x) + std::string(". ") + i.attribute("Text").as_string() + newL);

			//TEMP
			sf::Text response;
			response.setCharacterSize(22);
			response.setFillColor(sf::Color::White);
			response.setFont(font);
			response.setString(i.attribute("Text").as_string());
			response.setPosition(_dialogTextBoxP.getPosition().x, _dialogTextBoxP.getPosition().y + ((x-1) * response.getGlobalBounds().height) + (4 * (x-1)));

			_responses.push_back(response);
		}
	}

	std::cout << tempResponses << std::endl;
}

void DialogueSystem::updateDialogue(Entity* entity)
{
	unsigned int responseNum = entity->get<DialogueComponent>()->_selectedResponseNum;

	unsigned int responses = 1;

	for (pugi::xml_node i = responseRoot.first_child(); i; i = i.next_sibling())
		if (isDialogueConditionsMet(entity, &i))
		{
			if (responseNum == responses)
			{
				convoRoot = convoRoot.parent().find_child_by_attribute("DialogEntry", "ID", i.attribute("DestinationDialogID").value());
				std::cout << convoRoot.first_child().first_child().child("DialogueText").text().as_string() << std::endl;
				this->_dialogText.setString(convoRoot.first_child().first_child().child("DialogueText").text().as_string());//set gui text

				checkForActions(entity);
				checkForQuests(entity->get<QuestComponent>());
				processResponses(entity);
			}
			responses++;
		}
}

bool DialogueSystem::isEndOfConversation()
{
	if (convoRoot.attribute("IsEnd").as_bool() == true)
		return true;

	return false;
}

void DialogueSystem::checkForQuests(QuestComponent* entQuestComp)
{
	if (convoRoot.attribute("QuestActivator").as_bool() == true)
	{
		entQuestComp->activateQuest(convoRoot.child("OutgoingLinks").child("Link").attribute("ActivatedQuestID").as_uint());

		_questUpdated.notify(*entQuestComp->getOwnerEntity(), "TEST_QUEST");
	}
	else if (convoRoot.attribute("QuestUpdater").as_bool() == true)
	{
		entQuestComp->progressQuest(convoRoot.child("Fields").find_child_by_attribute("Field", "Type", "Quest").child("NextQuestID").text().as_uint(), convoRoot.child("Fields").find_child_by_attribute("Field", "Type", "Quest").child("NextQuestStage").text().as_uint());
	}
}

void DialogueSystem::checkForActions(Entity* entity)
{
	if (convoRoot.attribute("ActionRequired").as_bool() == true)
	{
		for (pugi::xml_node i = convoRoot.child("Fields").find_child_by_attribute("Field", "Type", "Action").first_child(); i; i = i.next_sibling())
		{
			std::cout << "TESITNG: " << i.attribute("ActionType").as_string() << std::endl;
			std::string action = i.attribute("ActionType").as_string();

			if (action == "RemoveItemFromInventory")
			{
				std::cout << "TESITNG: " << std::endl;
				entity->get<InventoryComponent>()->removeEntity(i.attribute("ItemTypeID").as_string());
				continue;
			}
		}
	}
}