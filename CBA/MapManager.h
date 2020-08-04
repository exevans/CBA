#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "pugixml.hpp"
#include <SFML/Graphics.hpp>

struct layerDetails {
	std::string tilesetImgPath = "";
	unsigned int firstgId = 0;
	unsigned int tileWidth = 0;
};

class MapManager : public sf::Drawable, sf::Transformable
{
public:
	MapManager();
	void loadMap(std::string);

private:
	void loadLayer(pugi::xml_node, layerDetails, int);
	void setUpMapLayer(std::string, sf::Vector2u, unsigned int, unsigned int);
	virtual void draw(sf::RenderTarget &, sf::RenderStates) const;

	std::vector<std::string> _tileSets;
	std::vector <int> level;
	std::vector<sf::VertexArray> _mapVertex;
	sf::Texture mapTexSet;
	unsigned int _width = 0, _height = 0;
	unsigned int tileSize = 0;
};

#endif