#pragma once

#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Object.hpp"
#include "ObjectParser.hpp"

class Scene
{
	public:

	Scene() = default;
	~Scene() = default;
	Scene(const std::string& filePath);
	Scene(const Scene& other) = delete;
	Scene& operator=(const Scene& other) = delete;

	void	addObject(const Object& obj) { objects.emplace_back(std::move(obj)); }

	private:
	
	std::vector<Object>	objects;
};
