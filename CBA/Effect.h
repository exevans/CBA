#ifndef EFFECT_H
#define EFFECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Effect : public sf::Drawable
{
public:
	void load();
	void update(float, float, float);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Shader& getShader();
protected:
	Effect(const std::string&);
	sf::Shader _shader;

private:
	virtual bool onLoad() = 0;
	virtual void onUpdate(float, float, float) = 0;
	virtual void onDraw(sf::RenderTarget&, sf::RenderStates) const = 0;

	bool _isLoaded;
	std::string _name;
};


class Pixelate : public Effect
{
public:
	Pixelate();
	virtual bool onLoad() override;
	virtual void onUpdate(float, float, float) override;
	virtual void onDraw(sf::RenderTarget&, sf::RenderStates) const override;

private:
};

#endif