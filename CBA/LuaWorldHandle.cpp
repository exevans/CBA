#include "LuaWorldHandle.h"
#include "SharedContext.h"
#include "MapManager.h"
#include "SystemManager.h"
#include "TextManager.h"
#include "LuaManager.h"

LuaWorldHandle::LuaWorldHandle(SharedContext* sharedCon) :_sharedCon(sharedCon) {
	
}

void LuaWorldHandle::loadLevel(const std::string& path) {
	_sharedCon->_entityManager->requestLoadLevel(path);
}

void LuaWorldHandle::reloadLevel() {
	_sharedCon->_entityManager->requestReloadLevel();
}

int LuaWorldHandle::getIdFromFile(int fileId) {
	return _sharedCon->_entityManager->getEntityIdFromFileId(fileId);
}

int LuaWorldHandle::spawn(const std::string& path, const std::string& table) {
	int newEntId = _sharedCon->_entityManager->addEntity(path, table);
	return newEntId;
}

void LuaWorldHandle::startDialogue(const EntityID& ent) {
	EntityManager* entManager = _sharedCon->_entityManager;
	if (entManager->hasComponent(ent, ComponentType::Dialogue)) {
		entManager->getComponent<DialogueComponent>(ent, ComponentType::Dialogue)->_inConversation = true;
		entManager->getComponent<DialogueComponent>(ent, ComponentType::Dialogue)->_otherEntity = 0;
		entManager->getComponent<DialogueComponent>(0, ComponentType::Dialogue)->_inConversation = true;
		entManager->getComponent<DialogueComponent>(0, ComponentType::Dialogue)->_otherEntity = ent;
	}
	else
	{
		std::cout << "LuaEntityHandle doesn't have a Dialogue component" << std::endl;
	}
}

void LuaWorldHandle::addTextMessage(const EntityID& ent, const std::string& text, const sf::Vector2f& positionOffset, const float timeLimit) {
	PositionComponent* posComp = _sharedCon->_entityManager->getComponent<PositionComponent>(ent, ComponentType::Position);
	_sharedCon->_textManager->addTextMessage(ent, text, timeLimit, posComp->_position + positionOffset);
}

void LuaWorldHandle::removeTextMessage(const EntityID& ent) {
	_sharedCon->_textManager->removeTextMessage(ent);
}

float LuaWorldHandle::getDistanceBetweenPositions(sf::Vector2f firstEntPos, sf::Vector2f otherEntPos)
{
	sf::Vector2f dist;
	dist = otherEntPos - firstEntPos;

	float totalDist = sqrt((dist.x * dist.x) + (dist.y * dist.y));

	return totalDist;
}

void LuaWorldHandle::test() {
	std::cout << "got herefsefs" << std::endl;
}