#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>
#include <memory>

namespace Shader
{
	enum ShaderType{Fragment, Vertex, Both};
}

class ShaderManager
{
public:
	ShaderManager();
	//static void addShader(std::string&, std::string&, std::string&, Shader::ShaderType);
	static sf::Shader* getShader(std::string const&, std::string const&, std::string const&);

private:
	static std::map<std::string, std::unique_ptr<sf::Shader>> _shaders;

	static ShaderManager* sInstance;
};

#endif