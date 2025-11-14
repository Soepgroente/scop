#include "Scene.hpp"

Scene::Scene(const std::string& filePath)
{
	ObjectParser::addObjectsFromFile(filePath, *this);
}
