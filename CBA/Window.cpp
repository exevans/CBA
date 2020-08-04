#include "Window.h"

Window::Window() {
	this->setUp("UnNamed Window", sf::Vector2u(1920, 1080));
}

Window::Window(const std::string& title, const sf::Vector2u& size)  {
	this->setUp(title, size);
}

Window::~Window() {
	this->destroy();
}

void Window::setUp(const std::string& title, const sf::Vector2u& size) {
	_windowTitle = title;
	_windowSize = size;
	_isFullScreen = false;
	_isDone = false;

	this->create();
}

void Window::create() {
	auto style = (_isFullScreen ? sf::Style::Fullscreen : sf::Style::Default);
	_window.create(sf::VideoMode(_windowSize.x, _windowSize.y), _windowTitle, style);
	//_window.setVerticalSyncEnabled(true);
}

void Window::destroy() {
	_window.close();
}

void Window::update() {
	sf::Event event;
	while (_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			_isDone = true;
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5) {
			this->toggleFullScreen();
		}
		_eventManager.handleEvent(event);
	}
	_eventManager.update();
}

void Window::toggleFullScreen() {
	_isFullScreen = !_isFullScreen;
	destroy();
	create();
}

void Window::close() {
	_isDone = true;
}

void Window::beginDraw() {
	_window.clear();
}

void Window::endDraw() {
	_window.display();
}

EventManager* Window::getEventManager() {
	return &_eventManager;
}


bool Window::isDone() {
	return _isDone;
}

bool Window::isFullScreen() {
	return _isFullScreen;
}

sf::Vector2u Window::getWinSize() {
	return _windowSize;
}

sf::RenderWindow* Window::getRenderWindow() {
	return &_window;
}

sf::Vector2f Window::getMouseWorldPos() {
	sf::Vector2i pixPos = sf::Mouse::getPosition(_window);
	sf::Vector2f worldPos = _window.mapPixelToCoords(pixPos);
	return worldPos;
}

sf::FloatRect Window::getViewSpace() {
	sf::Vector2f viewCentre = _window.getView().getCenter();
	sf::Vector2f viewSize = _window.getView().getSize();
	sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
	sf::FloatRect viewSpace(viewCentre - viewSizeHalf, viewSize);

	return viewSpace;
}

void Window::draw(const sf::Drawable& object) {
	_window.draw(object);
}