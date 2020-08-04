#include "QuestManager.h"
#include <iostream>
#include "FontManager.h"
#include "DialogueManager.h"
#include <string>
#include <map>
#include "pugixml.hpp"
#include "Entity.h"

pugi::xml_document QuestManager::_questDoc;	//temp

QuestManager::QuestManager()
{
	_questTextBox.setSize(sf::Vector2f(350.f, 50.f));
	_questTextBox.setFillColor(sf::Color::Black);
	_questTextBox.setOutlineColor(sf::Color::White);
	_questTextBox.setOutlineThickness(2.f);
	//_questTextBox.setPosition(GameManager::_view.getSize().x - _questTextBox.getGlobalBounds().width + 10, 10.f);

	_questDescription.setCharacterSize(22);
	_questDescription.setFillColor(sf::Color::White);
	_questDescription.setPosition(_questTextBox.getPosition());

	//font.loadFromFile("Assets/Fonts/BERNHC.ttf");
	FontManager::addFont(std::string("Assets/Fonts/BERNHC.ttf")); 
	_questDescription.setFont(FontManager::getFontRef(std::string("Assets/Fonts/BERNHC.ttf")));
	_questDescription.setString("Test Quest");

	_questDoc.load_file("Assets/Data/Quests/Quests_0.xml");
}

void QuestManager::onNotify(Entity* entity, std::string eventType, unsigned int questID)
{
	if (eventType == "EVENT_QUEST_ACTIVATED")
	{
		std::cout << "New QUEST HAS BEGUN" << std::endl;
		this->activateQuest(questID);
	}
}

void QuestManager::onNotify(Entity* entity, std::string eventType, unsigned int questID, unsigned int stageID)
{
	if (eventType == "EVENT_QUEST_UPDATED")
	{
		std::cout << "New QUEST HAS BEEN UPDATED" << std::endl;
		this->progressQuest(questID, stageID);
	}
}

void QuestManager::draw(sf::RenderWindow & win)
{
	if (this->_showingQuest)
	{
		if (_clock.getElapsedTime().asSeconds() < 4)
		{
			win.setView(win.getDefaultView());
			win.draw(_questTextBox);
			win.draw(_questDescription);
		}
		else
			this->_showingQuest = false;
	}

}

void QuestManager::saveData()
{
	pugi::xml_document levelDoc;
	levelDoc.load_file("Assets/Data/Saves/TempObjectData.xml");

	pugi::xml_node questsRoot = levelDoc.child("Quests");	//get the current level ref
	for (pugi::xml_node quest = questsRoot.first_child();quest;)	//remove all quests
	{
		pugi::xml_node next = quest.next_sibling();
		quest.parent().remove_child(quest);
		quest = next;
	}

	for (pugi::xml_node docQuest = _questDoc.first_child().first_child(); docQuest; docQuest = docQuest.next_sibling())
	{
		if (docQuest.attribute("Activated").as_bool() == true)
		{
			questsRoot.append_child("Quest");

			pugi::xml_node newQuest = questsRoot.last_child();

			newQuest.append_attribute("ID").set_value(docQuest.attribute("ID").value());
			newQuest.append_attribute("CurrentStage").set_value(docQuest.attribute("CurrentStage").value());

			std::cout << docQuest.attribute("CurrentStage").as_uint() << std::endl;
	
			//newQuest.append_attribute("Finished").set_value(docQuest.attribute("CurrentStage").value());
		}
	}
	
	levelDoc.save_file("Assets/Data/Saves/TempObjectData.xml");
}

void QuestManager::loadData()
{
	pugi::xml_document levelDoc;
	levelDoc.load_file("Assets/Data/Saves/TempObjectData.xml");

	pugi::xml_node questsRoot = levelDoc.child("Quests");
	for (pugi::xml_node quest = questsRoot.first_child(); quest; quest = quest.next_sibling())
	{
		pugi::xml_node docQuest = _questDoc.child("Quests").find_child_by_attribute("Quest", "ID", quest.attribute("ID").as_string());
		docQuest.attribute("Activated").set_value(true);
		docQuest.attribute("CurrentStage").set_value(quest.attribute("CurrentStage").as_uint());

		if (docQuest.child("QuestStages").find_child_by_attribute("QuestStage", "StageID", quest.attribute("CurrentStage").as_string()).attribute("IsEnd").as_bool() == true)
		{
			docQuest.attribute("Finished").set_value(true);
		}
	}
}

void QuestManager::activateQuest(unsigned int questId)
{
	std::string QuestIdS = std::to_string(questId);
	char const* QuestIdC = (QuestIdS).c_str();

	_QuestRoot = _questDoc.child("Quests").find_child_by_attribute("Quest", "ID", QuestIdC).first_child().first_child();

	if (_QuestRoot.parent().parent().attribute("Activated").as_bool() == false)
	{
		_QuestRoot.parent().parent().attribute("Activated").set_value(true);

		std::cout << "New Quest: " << _QuestRoot.parent().parent().attribute("QuestTitle").as_string() << ", " << _QuestRoot.child("Fields").find_child_by_attribute("Field", "Type", "Text").child("Objective").text().as_string() << std::endl;
		_questDescription.setString(_QuestRoot.child("Fields").find_child_by_attribute("Field", "Type", "Text").child("Objective").text().as_string());
		_clock.restart();
		this->_showingQuest = true;
		std::cout << "Quest id = " << QuestIdC << std::endl;
	}
}

void QuestManager::progressQuest(unsigned int QuestID, unsigned int NewStageID)
{
	std::string QuestIdS = std::to_string(QuestID);
	char const* QuestIdC = (QuestIdS).c_str();

	std::string stageIdS = std::to_string(NewStageID);
	char const* stageIdC = (stageIdS).c_str();

	_QuestRoot = _questDoc.child("Quests").find_child_by_attribute("Quest", "ID", QuestIdC).first_child().find_child_by_attribute("QuestStage", "StageID", stageIdC);
	//_QuestText.setString(_QuestRoot.child("Fields").find_child_by_attribute("Field", "Type", "Text").child("Objective").text().as_string());
	_questDescription.setString(_QuestRoot.child("Fields").find_child_by_attribute("Field", "Type", "Text").child("Objective").text().as_string());
	std::cout << "Quest update: " << _QuestRoot.parent().parent().attribute("QuestTitle").as_string() << ", " << _QuestRoot.child("Fields").find_child_by_attribute("Field", "Type", "Text").child("Objective").text().as_string() << std::endl;

	_QuestRoot.parent().parent().attribute("CurrentStage").set_value(stageIdC);
	_clock.restart();
	this->_showingQuest = true;

	if (_QuestRoot.attribute("IsEnd").as_bool() == true)
	{
		std::cout << "Current Quest Finished";
		_QuestRoot.parent().parent().attribute("Finished").set_value(true);
	}
}

bool QuestManager::IsQuestActivated(std::string id)
{
	if (_questDoc.child("Quests").find_child_by_attribute("Quest", "ID", id.c_str()).attribute("Activated").as_bool() == true)
		return true;
	return false;
}

bool QuestManager::IsQuestStageReached(std::string id, int stageID)
{
	if (_questDoc.child("Quests").find_child_by_attribute("Quest", "ID", id.c_str()).attribute("CurrentStage").as_uint() >= stageID)
		return true;
	return false;
}

bool QuestManager::isQuestFinished(std::string id)
{
	if (_questDoc.child("Quests").find_child_by_attribute("Quest", "ID", id.c_str()).attribute("Finished").as_bool() == true)
		return true;
	return false;
}
