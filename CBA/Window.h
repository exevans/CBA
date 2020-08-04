#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <SFML\Graphics.hpp>
#include "EventManager.h"

class Window {
public:
	Window();
	Window(const std::string&, const sf::Vector2u&);
	~Window();

	void beginDraw();	//clear
	void endDraw();		//render

	void update();

	EventManager* getEventManager();
	bool isDone();	//ready to close
	bool isFullScreen();
	sf::RenderWindow* getRenderWindow();
	sf::Vector2u getWinSize();
	sf::Vector2f getMouseWorldPos();
	sf::FloatRect getViewSpace();

	void toggleFullScreen();
	void close();

	void draw(const sf::Drawable&);	//draw a object to the screen
private:
	void setUp(const std::string&, const sf::Vector2u&);
	void destroy();
	void create();

	EventManager _eventManager;
	sf::RenderWindow _window;
	sf::Vector2u _windowSize;
	std::string _windowTitle;
	bool _isDone;
	bool _isFullScreen;
};

#endif