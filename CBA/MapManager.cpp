#include "MapManager.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "GameManager.h"

MapManager::MapManager()
{
	
}

void MapManager::loadMap(std::string path)
{
	_mapVertex.clear();
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());

	if (!result)
	{
		std::cout << "XML [" << path << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
	}
	else
	{
		std::cout << "loaded" << std::endl;
	}

	pugi::xml_node mapRoot = doc.child("map");
	
	this->_width = mapRoot.attribute("width").as_int();
	this->_height = mapRoot.attribute("height").as_int();
	this->level.resize(this->_width * this->_height);
	layerDetails layerDetails;

	for (pugi::xml_node tileSet = mapRoot.child("tileset"); tileSet.attribute("source"); tileSet = tileSet.next_sibling()) {
		//_tileSets.push_back(tileSet.attribute("source").as_string());
		pugi::xml_document tilesetDoc;
		std::string tileSource = "Assets/Maps/";
		tileSource += tileSet.attribute("source").as_string();
		tilesetDoc.load_file(tileSource.c_str());

		layerDetails.tilesetImgPath = tilesetDoc.child("tileset").child("image").attribute("source").as_string();
		layerDetails.tileWidth = tilesetDoc.child("tileset").attribute("tilewidth").as_uint();
		layerDetails.firstgId = tileSet.attribute("firstgid").as_uint();
	}

	for (pugi::xml_node layer = mapRoot.find_child_by_attribute("layer", "name", "Tile Layer 1"); layer.attribute("width"); layer = layer.next_sibling()) {

		std::string name = layer.attribute("name").as_string();
		if (name == "Collision" || name == "Lighting") { continue; }

		this->_mapVertex.push_back(sf::VertexArray());
		this->loadLayer(layer, layerDetails, _mapVertex.size()-1);
	}
}

void MapManager::loadLayer(pugi::xml_node layer, layerDetails layerDetails, int layerNum)
{
	if (!layer.empty())
	{
		pugi::xml_node mapData = layer.child("data");

		std::string data = mapData.text().as_string();
		std::stringstream ss(data);
		std::vector<int> vect;
		int i;
		while (ss >> i)
		{
			vect.push_back(i);

			if (ss.peek() == ',')
				ss.ignore();
		}

		for (unsigned int j = 0; j < _height; j++)
			for (unsigned int i = 0; i < _width; i++)
			{
				level[i + (j * _width)] = vect[i + (j* _width)];
			}

		std::string tilesetImgPath = layerDetails.tilesetImgPath; //mapTileset.child("image").attribute("source").as_string();
		tilesetImgPath.replace(0, 2, "Assets");
		this->tileSize = layerDetails.tileWidth; //mapTileset.attribute("tilewidth").as_uint();
		unsigned int firstId = layerDetails.firstgId; //mapTileset.attribute("firstgid").as_int();
		this->setUpMapLayer(tilesetImgPath, sf::Vector2u(layerDetails.tileWidth, layerDetails.tileWidth), firstId, layerNum);
	}
}

void MapManager::setUpMapLayer(std::string tileTexPath, sf::Vector2u tileSize, unsigned int firstid, unsigned int layerNum)
{
	if (!mapTexSet.loadFromFile(tileTexPath))
		std::cout << "Could not load textures" << std::endl;
	else
	{
		_mapVertex[layerNum].setPrimitiveType(sf::Quads);
		_mapVertex[layerNum ].resize(_width * _height * 4);

			for (unsigned int j = 0; j < _height; j++)
				for (unsigned int i = 0; i < _width; i++)
				{
					int tileValue = level[i + (j * _width)];
					if (tileValue > 0)	// 0 means no texture
					{
						tileValue -= firstid;	//normalise value for tileset

						int numPerRow = (mapTexSet.getSize().x / this->tileSize);
						int tv = floor(tileValue / numPerRow);
						int tu = tileValue - (tv * numPerRow);

						sf::Vertex* quad = &_mapVertex[layerNum][(i + (j * _width)) * 4];

						quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
						quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
						quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
						quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

						quad[0].texCoords = sf::Vector2f(tu * this->tileSize, tv * this->tileSize);
						quad[1].texCoords = sf::Vector2f((tu + 1) * this->tileSize, tv * this->tileSize -0.0075);
						quad[2].texCoords = sf::Vector2f((tu + 1) * this->tileSize-0.0075, (tv + 1) * this->tileSize-0.0075);
						quad[3].texCoords = sf::Vector2f(tu * this->tileSize-0.0075, (tv + 1) * this->tileSize);
					}
				}
	}
}

void MapManager::draw(sf::RenderTarget & win, sf::RenderStates states) const
{
	states.transform *= getTransform();

	states.texture = &mapTexSet;

	for (auto& it : _mapVertex)
	{
		win.draw(it, states);
	}
}