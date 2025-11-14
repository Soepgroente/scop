#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "Scene.hpp"
#include "Object.hpp"

class ObjectParser
{
	public:

	static void	addObjectsFromFile(const std::string& objFilePath, Scene& scene);

	private:

	ObjectParser() = delete;
	~ObjectParser() = delete;
};