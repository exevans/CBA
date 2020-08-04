#include "AnimationSystem.h"

#include "Component.h"
#include "EntityMessage.h"
#include "Entity.h"
#include <iostream>

AnimationSystem::AnimationSystem(SystemManager* sysManager)
	:System(sysManager, SystemType::Animation)
{
	addRequiredComponentType(ComponentType::Animation);
	//addRequiredComponentType(ComponentType::Texture);
	//addRequiredComponentType(ComponentType::Movement);

	_systemManager->getMessageHandler()->subscribe(EntityMessage::Direction_Changed, this);
}

void AnimationSystem::update(float deltaTime)
{
	EntityManager* entManager = _systemManager->getEntityManager();
	for (auto ent : _entities) {
		//auto moveComp = entManager->getComponent<MovementComponent>(ent, ComponentType::Movement);
		//auto texComp = entManager->getComponent<TextureComponent>(ent, ComponentType::Texture);
		auto aniComp = entManager->getComponent<AnimationComponent>(ent, ComponentType::Animation);

		/*if (aniComp->_playing)	
		{
			aniComp->_elapsedTime += deltaTime;
			if (aniComp->_elapsedTime > aniComp->_animationRate)
			{
				if (aniComp->_currentFrame != aniComp->_endFrame)
					aniComp->_currentFrame++;
				else if (aniComp->_loop)
					aniComp->_currentFrame = aniComp->_startFrame;	//needs to be changed to start pos

				texComp->_sprite.setTextureRect(sf::IntRect(aniComp->_currentFrame * aniComp->_spriteSize.x, aniComp->_startRow * aniComp->_spriteSize.y, aniComp->_spriteSize.x, aniComp->_spriteSize.y));

				aniComp->_elapsedTime = 0;
			}
		}*/
		aniComp->_spriteSheet.update(deltaTime);
	}
}

void AnimationSystem::handleEvent(const EntityID& ent, const EntityEvent& entEvent) {
	/*EntityManager* entManager = _systemManager->getEntityManager();
	AnimationComponent* aniComp = _systemManager->getEntityManager()->getComponent<AnimationComponent>(ent, ComponentType::Animation);

	switch (entEvent) {
	case EntityEvent::Became_Idle:
		aniComp->_spriteSheet.setAnimation("Idle", false, true);
		break;
	}*/
}

void AnimationSystem::onNotify(const Message& message) {
	AnimationComponent* aniComp = _systemManager->getEntityManager()->getComponent<AnimationComponent>(message._receiver, ComponentType::Animation);
	if (message._mType == (int)EntityMessage::Direction_Changed) {
		aniComp->_spriteSheet.setDirection((Direction)message._int);
	}
}
