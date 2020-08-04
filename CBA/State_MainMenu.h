#ifndef STTAE_MAIN_MENU_H
#define STATE_MAIN_MENU_H

#include <SFML/Graphics.hpp>

#include "State.h"

class EventDetails;

class State_MainMenu : public State {
public:
	State_MainMenu(StateManager* sManager) : State(sManager){};
	void onCreate() override;
	void onDestroy() override;

	void activate() override;
	void deactivate() override;

	void update(float) override;
	void draw() override;

	void mouseClicked(EventDetails*);
private:
	std::vector<sf::Text> _texts;
	std::vector<sf::RectangleShape> _shapes;
	sf::Texture _introTexture;
	sf::Sprite _introSprite;
	sf::Texture _backgroundTexture;
	sf::Sprite _backgroundSprite;
	/*sf::RectangleShape _playShape;
	sf::RectangleShape _optionsShape;
	sf::RectangleShape _exitShape;
	sf::Text _playText;
	sf::Text _optionsText;
	sf::Text _quitText;*/
	sf::Font _font;
	
	std::vector<std::pair<sf::Texture, sf::Sprite>> _backgroundLayer;
};

#endif
