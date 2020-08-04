#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class Window;

using Wall = std::pair<sf::Vector2u, sf::Vector2u>;

class Light{
public:
	Light() {};
	Light(const sf::Vector2f& position, std::string& texture, const sf::Color colour, float);
	void addConfirmedCorner(const sf::Vector2f&);
	void generateLightingShape();
	void setPosition(const sf::Vector2f&);
	sf::Vector2f getPosition();
	float getRadius();
	sf::Sprite& getLightSprite();
	sf::VertexArray& getLightMap();

private:
	sf::Sprite _shape;
	sf::Vector2f _position;
	float _radius;

	sf::VertexArray _vertex;
	std::vector<sf::Vector2u> _corners;
	std::vector<sf::Vector2f> _confirmedCorners;
};

class LightManager {
public:
	LightManager();
	~LightManager();

	void loadLightMap(const std::string&);
	void addLight(const sf::Vector2f, std::string&, const sf::Color, const float, bool, unsigned int = 0);
	void updateDynamicLightPosition(unsigned int, const sf::Vector2f);
	void update(const float);
	void draw(Window*);
	void adder();
private:
	void purge();
	void addCorners(const sf::Vector2u);
	void attemptToAddCorner(const sf::Vector2u);
	void removeSurroundedCorners();
	bool raycastForCollision(const sf::Vector2f, const sf::Vector2u);
	sf::Vector2f raycastForExtensions(const sf::Vector2f, const sf::Vector2u);
	bool validForExtensions(const sf::Vector2u, const sf::Vector2f);

	std::map<unsigned int, Light> _lights;
	std::vector<unsigned int> _dynamicLightsKey;

	sf::Vector2i _cellArraySize;
	std::vector<bool> _tiles;
	std::vector<sf::Vector2u> _corners;
	sf::Color _mapAmbientColour;

	sf::RenderTexture _backLight;
	sf::RenderTexture _lightMap;
	sf::RenderTexture _lightShapeTextureMap;
	sf::RenderTexture _lightingFinal;
	unsigned int _lightCounter;
};

#endif