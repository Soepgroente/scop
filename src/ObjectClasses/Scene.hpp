#pragma once

#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Object.hpp"

class Scene
{
	public:

	Scene() = default;
	~Scene() = default;
	Scene(const Scene& other) = delete;
	Scene& operator=(const Scene& other) = delete;

	void	addObject(const Object& obj) { objects.emplace_back(std::move(obj)); }
	size_t	size() const { return objects.size(); }
	const Object&	getObject(size_t index) const { return objects.at(index); }

	private:
	
	std::vector<Object>	objects;
};
