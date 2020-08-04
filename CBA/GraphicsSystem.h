#ifndef GRAPHICSSYS_H
#define GRAPHICSSYS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Window.h"
#include "System.h"

class GraphicsSystem : public System
{
public:
	GraphicsSystem(SystemManager*);
	//void addIDToLayerVect(unsigned int);
	//virtual void checkForRemovedComponents() override;
	void draw(Window*);
	void update(float) override;
	void handleEvent(const EntityID&, const EntityEvent&) override;
	//void sortViews();

private:
	void sortDrawOrder();

	//std::shared_ptr<Window> _window;
	std::vector<unsigned int> _drawOrder;
	const unsigned int maxZOrder = 3;
};

#endif