#include "ShaderManager.h"

std::map<std::string, std::unique_ptr<sf::Shader>> ShaderManager::_shaders;

ShaderManager::ShaderManager()
{

}

/*void ShaderManager::addShader(std::string& shaderName, std::string& vertFilepath, std::string& fragFilepath, Shader::ShaderType shaderType)
{
	auto& shaderMap = sInstance->_shaders;
	auto pairFound = shaderMap.find(shaderName);

	if (pairFound == shaderMap.end())
	{
		sf::Shader newShader;
		switch (shaderType)
		{
		case Shader::Vertex:
			if (newShader.loadFromFile(vertFilepath, sf::Shader::Vertex))
			{
				shaderMap[shaderName] = std::unique_ptr<sf::Shader>(new sf::Shader(newShader));
				std::cout << "new shader added to shader manager" << std::endl;
			}
			else
			{
				std::cout << "Could not add shader " << shaderName << std::endl;
			}
			break;
		case Shader::Fragment:
				if (newShader.loadFromFile(fragFilepath, sf::Shader::Fragment))
				{
					shaderMap[shaderName] = std::unique_ptr<sf::Shader>(new sf::Shader(newShader));
					std::cout << "new shader added to shader manager" << std::endl;
				}
				else
				{
					std::cout << "Could not add shader " << shaderName << std::endl;
				}
			break;
		case Shader::Both:
			if (newShader.loadFromFile(fragFilepath,vertFilepath))
			{
				shaderMap[shaderName] = std::unique_ptr<sf::Shader>(new sf::Shader(newShader));
				std::cout << "new shader added to shader manager" << std::endl;
			}
			else
			{
				std::cout << "Could not add shader " << shaderName << std::endl;
			}
			break;
		}
	}
}*/

sf::Shader* ShaderManager::getShader(std::string const& shaderName, std::string const& vertFilepath, std::string const& fragFilepath)
{
	auto& shaderMap = sInstance->_shaders;
	auto pairFound = shaderMap.find(shaderName);

	if (pairFound == shaderMap.end())
	{
		auto& shader = (shaderMap[shaderName] = std::unique_ptr<sf::Shader>(new sf::Shader()));

		if (vertFilepath != "" && fragFilepath != "")
			shader->loadFromFile(vertFilepath, fragFilepath);
		else
			shader->loadFromFile(fragFilepath, sf::Shader::Fragment);
		std::cout << "new shader added to shader manager" << std::endl;

		return shader.get();
	}
	else
	{
		return pairFound->second.get();
	}
}