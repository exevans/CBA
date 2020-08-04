#include <memory>
#include "GraphicsSystem.h"
#include "Component.h"
#include "Entity.h"
#include "Component.h"
#include "ShaderManager.h"
#include "SystemManager.h"

GraphicsSystem::GraphicsSystem(SystemManager* sysManager) : System(sysManager, SystemType::Graphics)
{
	addRequiredComponentType(ComponentType::Position);
	addRequiredComponentType(ComponentType::Texture);
}

/*void GraphicsSystem::addIDToLayerVect(unsigned int id)
{
	_compVect.push_back(id);
}

void GraphicsSystem::checkForRemovedComponents()
{
	//System::checkForRemovedComponents();
	for (std::map<std::type_index, std::map<unsigned int, Component*>>::iterator it = _components.begin(); it != _components.end(); ++it)
	{
		std::map<unsigned int, Component*>::iterator ot = it->second.begin();
		while (ot != it->second.end())
		{
			if (!ot->second->isActive())	//not active
			{
				for (size_t i = 0; i < _compVect.size(); ++i)
				{
					if (i == ot->first)
						_compVect.erase(_compVect.begin() + i);
				}
				ot = it->second.erase(ot);
			}
			else
				++ot;
		}
	}
}*/

void GraphicsSystem::update(float deltaTime)
{
	//sortDrawOrder();
	EntityManager* entManager = _systemManager->getEntityManager();
	for (auto& ent : _entities) {
		if (!entManager->hasComponent(ent, ComponentType::Animation)) {
			TextureComponent* texComp = entManager->getComponent<TextureComponent>(ent, ComponentType::Texture);
			texComp->_sprite.setPosition(_systemManager->getEntityManager()->getComponent<PositionComponent>(ent, ComponentType::Position)->_position);
		}
		else {
			AnimationComponent* aniComp = entManager->getComponent<AnimationComponent>(ent, ComponentType::Animation);
			aniComp->_spriteSheet.setSpritePosition(_systemManager->getEntityManager()->getComponent<PositionComponent>(ent, ComponentType::Position)->_position);
		}
	}
}

void GraphicsSystem::draw(Window* window) {
	EntityManager* entManager = _systemManager->getEntityManager();
	for (auto& ent : _entities) {
		if (!entManager->hasComponent(ent, ComponentType::Animation)) {
			TextureComponent* texComp = entManager->getComponent<TextureComponent>(ent, ComponentType::Texture);
			//culling - only draw sprites in view
			if (texComp->_sprite.getPosition().x + texComp->_sprite.getGlobalBounds().width/2 < window->getViewSpace().left || texComp->_sprite.getPosition().y < window->getViewSpace().top || texComp->_sprite.getPosition().x > window->getViewSpace().left + window->getViewSpace().width || texComp->_sprite.getPosition().y > window->getViewSpace().top + window->getViewSpace().height) { continue; }

		}
		else {
			AnimationComponent* aniComp = entManager->getComponent<AnimationComponent>(ent, ComponentType::Animation);
			if (aniComp->_spriteSheet.getSpritePosition().x + aniComp->_spriteSheet.getSpriteSize().x / 2 < window->getViewSpace().left || aniComp->_spriteSheet.getSpritePosition().y < window->getViewSpace().top || aniComp->_spriteSheet.getSpritePosition().x > window->getViewSpace().left + window->getViewSpace().width || aniComp->_spriteSheet.getSpritePosition().y > window->getViewSpace().top + window->getViewSpace().height) { continue; }
			aniComp->_spriteSheet.draw(window->getRenderWindow());
		}
	}
}

void GraphicsSystem::handleEvent(const EntityID& ent, const EntityEvent& entEvent) {
	if (entEvent == EntityEvent::Moving_Left ||
		entEvent == EntityEvent::Moving_Right ||
		entEvent == EntityEvent::Moving_Up ||
		entEvent == EntityEvent::Moving_Down ||
		entEvent == EntityEvent::Spawned) {
		sortDrawOrder();
	}
}

void GraphicsSystem::sortDrawOrder()
{
	EntityManager* entManager = _systemManager->getEntityManager();
	std::sort(_entities.begin(), _entities.end(), [entManager](unsigned int val_1, unsigned int val_2) {
		if (!entManager->hasComponent(val_1, ComponentType::Collision) && entManager->hasComponent(val_2, ComponentType::Collision))
			return true;
		else if (!entManager->hasComponent(val_2, ComponentType::Collision))
			return false;

		CollisionComponent* firstI = entManager->getComponent<CollisionComponent>(val_1, ComponentType::Collision);
		CollisionComponent* secondI = entManager->getComponent<CollisionComponent>(val_2, ComponentType::Collision);
		return (firstI->_colRect.top + firstI->_colRect.height) < (secondI->_colRect.top + secondI->_colRect.height);
	});
}