#include "LightManager.h"
#include "Window.h"
#include <pugixml.hpp>
#include <sstream>
#include <limits>
#include <iostream>
#include <math.h>

Light::Light(const sf::Vector2f& position, std::string& texture, const sf::Color colour, float radius) :_position(position), _radius(radius) {

	TextureManager::addTexture(texture);

	_shape.setTexture(TextureManager::getTextureRef(texture));
	_shape.setOrigin(_shape.getGlobalBounds().width / 2, _shape.getGlobalBounds().height / 2);
	_shape.setPosition(position);
	_shape.setScale(radius/2, radius/2);
	_shape.setColor(colour); 
}

void Light::addConfirmedCorner(const sf::Vector2f& cornPos) {
	sf::Vector2f newDir = sf::Vector2f(cornPos) - _shape.getPosition();
	float newAngleVal = std::atan2(newDir.y, newDir.x);

	//if the list is currently empty just add the cirner
	if (_confirmedCorners.empty()) {
		_confirmedCorners.emplace_back(cornPos);
		return;
	}

	//find correct place to add corner
	for (int i = 0; i < _confirmedCorners.size(); i++) {
		sf::Vector2f dir = sf::Vector2f(_confirmedCorners[i]) - _shape.getPosition();
		float angle = std::atan2(dir.y, dir.x);

		if (newAngleVal < angle) {
			_confirmedCorners.insert(_confirmedCorners.begin() + i, cornPos);
			return;
		}
	}

	//if reached end of list just add it
	_confirmedCorners.emplace_back(cornPos);
}

void Light::generateLightingShape() {
	
	_vertex.setPrimitiveType(sf::PrimitiveType::TrianglesFan);

	//place center point of fan
	_vertex.append(sf::Vertex(_shape.getPosition(), sf::Color::White));
	for (auto& point : _confirmedCorners) {
		_vertex.append(sf::Vertex(sf::Vector2f(point), sf::Color::White));
	}
	//link up to first triangle
	_vertex.append(sf::Vertex(sf::Vector2f(_confirmedCorners[0]), sf::Color::White));
}

void Light::setPosition(const sf::Vector2f& pos) {
	_position = pos;
	_shape.setPosition(_position);
}

sf::Vector2f Light::getPosition() {
	return _position;
}

float Light::getRadius() {
	return _radius;
}

sf::Sprite& Light::getLightSprite() {
	return _shape;
}

sf::VertexArray& Light::getLightMap() {
	return _vertex;
}

LightManager::LightManager() : _mapAmbientColour(sf::Color(20, 20, 20)), _lightCounter(1000) {
	_lightMap.create(1920, 1080);
	_lightShapeTextureMap.create(1920, 2000);
	_lightingFinal.create(1920, 1920);
	_backLight.create(1920, 1080);
}

LightManager::~LightManager() {
	_lights.clear();
}

void LightManager::loadLightMap(const std::string& path) {

	purge();

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());

	if (!result)
	{
		std::cout << "XML [" << path << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
	}
	else
	{
		std::cout << "loaded LightMap" << std::endl;
	}

	pugi::xml_node mapRoot = doc.child("map");

	this->_cellArraySize.x = mapRoot.attribute("width").as_int();
	this->_cellArraySize.y = mapRoot.attribute("height").as_int();
	this->_tiles.clear();
	this->_tiles.resize(this->_cellArraySize.x * this->_cellArraySize.y, false);

	//attempt to load ambientlighting colour
	pugi::xml_node amLightCol = mapRoot.child("properties").find_child_by_attribute("property", "name", "ambientLighting");
	if (!amLightCol.empty()) {
		std::string data = amLightCol.attribute("value").as_string();
		std::stringstream ss(data);
		int r, g, b;
		ss >> r;
		ss >> g;
		ss >> b;

		_mapAmbientColour = sf::Color(r, g, b);
	}
	else {
		_mapAmbientColour = sf::Color(15, 15, 15);
	}

	pugi::xml_node tileSet = mapRoot.child("tileset");
	pugi::xml_document tilesetDoc;
	std::string tileSource = "Assets/Maps/";
	tileSource += tileSet.attribute("source").as_string();
	tilesetDoc.load_file(tileSource.c_str());

	pugi::xml_node collisionLayer = mapRoot.find_child_by_attribute("layer", "name", "Lighting");
	if (collisionLayer.empty()) { return; }

	pugi::xml_node mapData = collisionLayer.child("data");

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

	//check all solid lighting block corners
	for (unsigned int j = 0; j < _cellArraySize.y; j++) {
		for (unsigned int i = 0; i < _cellArraySize.x; i++)	{
			if (vect[i + (j * _cellArraySize.x)] != 0) {
				_tiles[i + (j * _cellArraySize.x)] = true;
				addCorners(sf::Vector2u(i, j));
			}
		}
	}

	//remove surrounded corners that can never be hit
	removeSurroundedCorners();

	std::string texpath = "Assets/Textures/Map/Lighting/light.png";

	//add static lights
	pugi::xml_node objects = mapRoot.find_child_by_attribute("objectgroup", "name", "Lights");
	for (pugi::xml_node object = objects.first_child(); object; object = object.next_sibling())
	{
		pugi::xml_node properties = object.child("properties");

		//adds light to position (corrects for circle middle)
		sf::Vector2f lightPos(object.attribute("x").as_float() + object.attribute("width").as_float() / 2, object.attribute("y").as_float() + object.attribute("width").as_float() / 2);
		float radius = properties.find_child_by_attribute("property", "name", "radius").attribute("value").as_float();

		sf::Color colour = sf::Color(properties.find_child_by_attribute("property", "name", "red").attribute("value").as_uint(), properties.find_child_by_attribute("property", "name", "green").attribute("value").as_uint(), properties.find_child_by_attribute("property", "name", "blue").attribute("value").as_uint());

		addLight(lightPos, texpath, colour, radius, false);
	}
}

void LightManager::purge() {
	_lights.clear();
	_corners.clear();
}

void LightManager::addCorners(sf::Vector2u blockPos) {
	//add to list of corners
	attemptToAddCorner(sf::Vector2u(blockPos.x * 128, blockPos.y * 128));
	attemptToAddCorner(sf::Vector2u((blockPos.x + 1) * 128, blockPos.y * 128));
	attemptToAddCorner(sf::Vector2u((blockPos.x + 1) * 128, (blockPos.y + 1) * 128));
	attemptToAddCorner(sf::Vector2u(blockPos.x * 128, (blockPos.y + 1) * 128));
}

void LightManager::attemptToAddCorner(sf::Vector2u cornerPos) {
	for (unsigned int i = 0; i < _corners.size(); i++) {
		if (_corners[i] == cornerPos) { return; }
	}

	_corners.emplace_back(sf::Vector2u(cornerPos.x, cornerPos.y));
}

void LightManager::removeSurroundedCorners() {
	int i = 0;
	while (i < _corners.size()) {
		sf::Vector2u squarePos(_corners[i].x / 128, _corners[i].y / 128);

		if (squarePos.x == 0 || squarePos.y == 0 || squarePos.x == _cellArraySize.x || squarePos.y == _cellArraySize.y) {
			//all edge corners are eliminated
			_corners.erase(_corners.begin() + i);
		}
		else if (_tiles[squarePos.x - 1 + ((squarePos.y - 1) * _cellArraySize.x)] && _tiles[squarePos.x + ((squarePos.y - 1) * _cellArraySize.x)] && _tiles[squarePos.x + (squarePos.y * _cellArraySize.x)] && _tiles[squarePos.x - 1 + (squarePos.y * _cellArraySize.x)]) {
			_corners.erase(_corners.begin() + i);
		}
		else {
			i++;
		}
	}
}

void LightManager::updateDynamicLightPosition(unsigned int lightId, sf::Vector2f newPos) {
	//get correct Light;
	Light& light = _lights[lightId];
	light.setPosition(newPos);
}

void LightManager::addLight(const sf::Vector2f position, std::string& texture, const sf::Color colour, const float radius, bool dynamic, unsigned int entId) {
	if (dynamic) {
		_lights[entId] = Light(position, texture, colour, radius);
		_dynamicLightsKey.push_back(entId);
	}
	else {
		_lights[_lightCounter] = Light(position, texture, colour, radius);
	}
	
	int lightId = _lights.size() - 1;

	for (auto& cornPos : _corners) {
		//check if corner is in radius
		if (cornPos.x < position.x - ((radius + 1) * 128) || cornPos.y < position.y - ((radius + 1) * 128) || cornPos.x > position.x + ((radius + 1) * 128) || cornPos.y > position.y + ((radius + 1) * 128)) {
			continue;
		}

		if (raycastForCollision(position, cornPos)) {
			//add corner to confirmed
			_lights[lightId].addConfirmedCorner(sf::Vector2f(cornPos));
			//check for extensions
			if (validForExtensions(cornPos, position)) {
				_lights[lightId].addConfirmedCorner(raycastForExtensions(position, cornPos));
			}
		}
	}

	_lights[lightId].generateLightingShape();

	++_lightCounter;
}

bool LightManager::raycastForCollision(const sf::Vector2f src, const sf::Vector2u dest) {
	//make line equation (y = mx + c)
	float eqM = ((src.y - dest.y) / (src.x - dest.x));
	float eqC = (src.y - (eqM * src.x));
	float testY = (eqM * src.x) + eqC;
	sf::Vector2i startSquare(src.x / 128, src.y / 128);
	sf::Vector2i currentSquare(startSquare);
	sf::Vector2i destinationSquare(dest.x / 128, dest.y / 128);

	if (destinationSquare.y > startSquare.y)
		currentSquare.y++;
	if (destinationSquare.x > startSquare.x)
		currentSquare.x++;

	while (currentSquare.x != destinationSquare.x) {
		//get relevant square
		float yInterceptFloatLocal = (eqM * currentSquare.x * 128) + eqC;
		int yIntercept =  (eqM * currentSquare.x /** 128*/) + (eqC/128);
		//int yInterceptLocal = yIntercept / 128;

		if (yIntercept >= 0 && yIntercept < _cellArraySize.y) {
			//if intersects with solid then it has collided and the destination isnt visible
			if (src.x > dest.x) {
				if (currentSquare.x == 0) { return false; }

				/*if (int(yInterceptFloatLocal) % 128 == 0) {
					if (_tiles[currentSquare.x -1 + ((yIntercept)* _cellArraySize.x)] == true) {
						return false;
					}
				}*/

				if (_tiles[currentSquare.x - 1 + ((yIntercept)* _cellArraySize.x)] == true) {
					return false;
				}
			}
			else {
				if (_tiles[currentSquare.x + ((yIntercept)* _cellArraySize.x)] == true) {
					return false;
				}
			}
		}

		if (currentSquare.x > destinationSquare.x) {
			currentSquare.x--;
		} else if (currentSquare.x < destinationSquare.x) {
			currentSquare.x++;
		} else {
			break;
		}
	}

	while (currentSquare.y != destinationSquare.y) {
		//get relevant square
		float xInterceptFloatLocal = ((currentSquare.y * 128) - eqC) / (eqM);
		int xIntercept = (currentSquare.y - eqC/128) / (eqM);

		if (xIntercept >= 0 && xIntercept < _cellArraySize.x) {
			//if intersects with solid then it has collided and the destination isnt visible
			if (src.y > dest.y) {
				//if (currentSquare.y == 0) { return false; }
				if (int(xInterceptFloatLocal) % 128 == 0) {
					if (_tiles[xIntercept - 1 + ((currentSquare.y - 1) * _cellArraySize.x)] == true) {
						return false;
					}
				}


				if (_tiles[xIntercept + ((currentSquare.y - 1) * _cellArraySize.x)] == true) {
					return false;
				}
			}
			else {
				if (int(xInterceptFloatLocal) % 128 == 0) {
					if (_tiles[xIntercept - 1 + ((currentSquare.y) * _cellArraySize.x)] == true) {
						return false;
					}
				}

				if (_tiles[xIntercept + (currentSquare.y * _cellArraySize.x)] == true) {
					return false;
				}
			}
		}

		if (currentSquare.y > destinationSquare.y) {
			currentSquare.y--;
		} else if (currentSquare.y < destinationSquare.y) {
			currentSquare.y++;
		} else {
			break;
		}
	}
	//corner is confirmed

	return true;
}

sf::Vector2f LightManager::raycastForExtensions(const sf::Vector2f src, const sf::Vector2u mid) {

	float eqM = ((src.y - mid.y) / (src.x - mid.x));
	float eqC = (src.y - (eqM * src.x));
	//float testY = (eqM * src.x) + eqC;
	sf::Vector2i startSquare(mid.x / 128, mid.y / 128);
	sf::Vector2i currentSquare(startSquare);

	float xInterceptFloat = std::numeric_limits<float>::max();
	float yInterceptFloat = std::numeric_limits<float>::max();

	/*if (mid.y > src.y)
		currentSquare.y++;*/
	if (mid.x > src.x)
		currentSquare.x++;


	while (currentSquare.x >= 0 && currentSquare.x < _cellArraySize.x) {

		if (currentSquare.x != startSquare.x) {
			float yInterceptFloatLocal = (eqM * currentSquare.x * 128) + eqC;
			int yIntercept = yInterceptFloatLocal / 128;

			if (yIntercept >= 0 && yIntercept < _cellArraySize.y) {
				//if intersects with solid then it has collided and the destination isnt visible
				if (src.x > mid.x) {
					//if (currentSquare.x == 0) { return false; }
					if (_tiles[currentSquare.x-1 + ((yIntercept)* _cellArraySize.x)] == true) {
						yInterceptFloat = yInterceptFloatLocal;
						break;
					}
				}
				else {
					if (_tiles[currentSquare.x + ((yIntercept)* _cellArraySize.x)] == true) {
						yInterceptFloat = yInterceptFloatLocal;
						break;
					}
				}
			}
		}

		if (mid.x > src.x) {
			currentSquare.x++;
		} else if (mid.x < src.x) {
			currentSquare.x--;
		}
	}

	while (currentSquare.y >= 0 && currentSquare.y < _cellArraySize.y) {
		if (currentSquare.y != startSquare.y) {
			float xInterceptFloatLocal = ((currentSquare.y * 128) - eqC) / (eqM);
			int xIntercept = xInterceptFloatLocal / 128;

			if (xIntercept >= 0 && xIntercept < _cellArraySize.x) {
				//if intersects with solid then it has collided and the destination isnt visible
				if (src.y > mid.y) {
					//if (currentSquare.x == 0) { return false; }
					if (_tiles[xIntercept + ((currentSquare.y-1) * _cellArraySize.x)] == true) {
						xInterceptFloat = xInterceptFloatLocal;
						break;
					}
				}
				else {
					if (_tiles[xIntercept + ((currentSquare.y) * _cellArraySize.x)] == true) {
						xInterceptFloat = xInterceptFloatLocal;
						break;
					}
				}
			}
		}

		if (mid.y > src.y) {
			currentSquare.y++;
		} else if (mid.y < src.y) {
			currentSquare.y--;
		}
	}

	//determine which point is closer
	float Xin = sqrt(((xInterceptFloat - src.x) * (xInterceptFloat - src.x)) + ((currentSquare.y*128 - src.y) * (currentSquare.y*128 - src.y)));
	float Yin = sqrt(((currentSquare.x*128 - src.x) * (currentSquare.x*128 - src.x)) + ((yInterceptFloat - src.y) * (yInterceptFloat - src.y)));

	//adds offset to ensure correct oredering
	sf::Vector2f output;
	if (Xin < Yin) {
		output = sf::Vector2f(xInterceptFloat, currentSquare.y * 128);
		if (output.y > src.y) {
			if (xInterceptFloat < src.x) {
				if (_tiles[startSquare.x + ((startSquare.y) * _cellArraySize.x)] == true) {
					output.x -= 0.01;
				}
				else {
					output.x += 0.01;
				}
			}
			else {
				if (_tiles[startSquare.x-1 + ((startSquare.y) * _cellArraySize.x)] == true) {
					output.x += 0.01;
				}
				else {
					output.x -= 0.01;
				}
			}
		}
		else {
			if (xInterceptFloat < src.x) {
				if (_tiles[startSquare.x-1 + ((startSquare.y) * _cellArraySize.x)] == true) {
					output.x += 0.01;
				}
				else {
					output.x -= 0.01;
				}
			}
			else {
				if (_tiles[startSquare.x + ((startSquare.y) * _cellArraySize.x)] == true) {
					output.x -= 0.01;
				}
				else {
					output.x += 0.01;
				}
			}
		}
	} else {
		output = sf::Vector2f(currentSquare.x * 128, yInterceptFloat);

		if (output.x > src.x) {
			if (yInterceptFloat < src.y) {
				if (_tiles[startSquare.x + ((startSquare.y) * _cellArraySize.x)] == true) {
					output.y -= 0.01;
				}
				else {
					output.y += 0.01;
				}
			}
			else {
				if (_tiles[startSquare.x + ((startSquare.y) * _cellArraySize.x)] == true) {
					output.y += 0.01;
				}
				else {
					output.y -= 0.01;
				}
			}
		}
		else {
			if (yInterceptFloat < src.y) {
				if (_tiles[startSquare.x + ((startSquare.y -1) * _cellArraySize.x)] == true) {
					output.y += 0.01;
				}
				else {
					output.y -= 0.01;
				}
			}
			else {
				if (_tiles[startSquare.x + ((startSquare.y) * _cellArraySize.x)] == true) {
					output.y -= 0.01;
				}
				else {
					output.y += 0.01;
				}
			}
		}

	}

	return output;
}

bool LightManager::validForExtensions(const sf::Vector2u cornerPos, const sf::Vector2f srcPos) {
	bool valid = true;

	if (cornerPos.x == 0 || cornerPos.y == 0 || cornerPos.x == _cellArraySize.x * 128 || cornerPos.y == _cellArraySize.y * 128) { return false; }

	sf::Vector2u sNum(cornerPos.x / 128, cornerPos.y / 128);

	sf::Vector2i offset(0, 0);
	if (srcPos.x > cornerPos.x) { offset.x = -1; }
	if (srcPos.y > cornerPos.y) { offset.y = -1; }
	
	if (_tiles[(sNum.x + offset.x) + ((sNum.y + offset.y) * _cellArraySize.x)] == true) { valid = false; }

	int cornersNum = 0;
	if (_tiles[sNum.x + (sNum.y * _cellArraySize.x)] == true) { cornersNum++; }
	if (_tiles[(sNum.x - 1) + (sNum.y * _cellArraySize.x)] == true) { cornersNum++; }
	if (_tiles[sNum.x + ((sNum.y -1) * _cellArraySize.x)] == true) { cornersNum++; }
	if (_tiles[(sNum.x - 1) + ((sNum.y - 1) * _cellArraySize.x)] == true) { cornersNum++; }

	if (cornersNum != 1) {
		valid = false;
	}

	return valid;
}

/*bool LightManager::valueBetween(const unsigned int value, const unsigned int bound1, const unsigned int bound2) {
	bool b1Lower = false;
	
	if (bound1 < bound2) {
		b1Lower = true;
	}

	bool valid = true;
	if (b1Lower && (value < bound1 || value > bound2)) {
		valid = false;
	}
	if (!b1Lower && (value > bound1 || valid < bound2)) {
		valid = false;
	}

	return valid;
}*/

/*void LightManager::sortConfirmedCorners(const sf::Vector2f srcPos) {
	std::vector<sf::Vector2u> orderedCorners;

	float pi = 3.14159265358979;

	while (!_confirmedCorners.empty()) {

		unsigned int smallestAngleCornId = 0;
		sf::Vector2f dir = sf::Vector2f(_confirmedCorners[0]) - srcPos;
		float smallestAngleVal = std::atan2(dir.y, dir.x) * 180 / pi;

		for (int i = 0; i < _confirmedCorners.size(); i++) {
			sf::Vector2f dir = sf::Vector2f(_confirmedCorners[i]) - srcPos;
			float angle = std::atan2(dir.y, dir.x) * 180 / pi;
			
			if (angle < smallestAngleVal) {
				smallestAngleCornId = i;
			}
		}

		orderedCorners.emplace_back(_confirmedCorners[smallestAngleCornId]);
		_confirmedCorners.erase(_confirmedCorners.begin() + smallestAngleCornId);
	}

	_confirmedCorners = orderedCorners;
}*/

/*sf::ConvexShape LightManager::generateLightingShape() {
	sf::ConvexShape polygon;

	polygon.setPointCount(_confirmedCorners.size());

	for (int i = 0; i < _confirmedCorners.size(); i++) {
		polygon.setPoint(i, sf::Vector2f(_confirmedCorners[i]));
	}

	polygon.setFillColor(sf::Color::White);

	return polygon;
}*/

void LightManager::adder() {
	//start
	int val = 0;
	val = val + 1;
	//end
}

void LightManager::update(const float deltaTime) {
	for (auto lightId : _dynamicLightsKey) {
		sf::Vector2f position = _lights[lightId].getPosition();
		float radius = _lights[lightId].getRadius();

		for (auto& cornPos : _corners) {
			//check if corner is in radius
			if (cornPos.x < position.x - ((radius + 1) * 128) || cornPos.y < position.y - ((radius + 1) * 128) || cornPos.x > position.x + ((radius + 1) * 128) || cornPos.y > position.y + ((radius + 1) * 128)) {
				continue;
			}

			if (raycastForCollision(position, cornPos)) {
				//add corner to confirmed
				_lights[lightId].addConfirmedCorner(sf::Vector2f(cornPos));
				//check for extensions
				if (validForExtensions(cornPos, position)) {
					_lights[lightId].addConfirmedCorner(raycastForExtensions(position, cornPos));
				}
			}
		}

		_lights[lightId].generateLightingShape();
	}
}

void LightManager::draw(Window* win) {
	_backLight.clear(_mapAmbientColour);

	for (auto& light : _lights) {
		//add each light sprite overaly to lightsSprite -- circuler sprite on black bckgd
		_lightMap.clear(sf::Color::Black);
		light.second.getLightSprite().setPosition(light.second.getPosition().x -win->getViewSpace().left, light.second.getPosition().y -win->getViewSpace().top); //test
		_lightMap.draw(light.second.getLightSprite());
		_lightMap.display();

		//lightsSprite  fills screen 
		sf::Sprite lightsSprite(_lightMap.getTexture());
		lightsSprite.setPosition(win->getViewSpace().left, win->getViewSpace().top); //test

		//draw overlay lightmap -- white in lightable regions from source (no fadeoff goes to infinity)
		_lightShapeTextureMap.clear(sf::Color::Black);
		_lightShapeTextureMap.draw(light.second.getLightMap());
		_lightShapeTextureMap.display();
		sf::Sprite lightShape(_lightShapeTextureMap.getTexture());
		//lightsSprite.setPosition(win->getViewSpace().left, win->getViewSpace().top); //test

		//produce black bckgd, and sprite where in a lightable region.
		_lightingFinal.clear();
		_lightingFinal.draw(lightsSprite);
		_lightingFinal.draw(lightShape, sf::BlendMultiply);
		_lightingFinal.display();

		sf::Sprite finalLighting(_lightingFinal.getTexture());
		finalLighting.setPosition(-win->getViewSpace().left, -win->getViewSpace().top);

		//add the light to the output
		_backLight.draw(finalLighting, sf::BlendAdd);
	}

	_backLight.display();
	sf::Sprite backLight(_backLight.getTexture());
	//fit viewspace
	backLight.setPosition(win->getViewSpace().left, win->getViewSpace().top);

	win->getRenderWindow()->draw(backLight, sf::BlendMultiply);

}