#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Material.hpp"

struct Faces
{
	int	faces[3];
};

struct Mesh
{
	std::vector<Vec4>	vertices;
	std::vector<Vec3>	normals;
	std::vector<Vec3>	textureCoords;
	std::vector<Faces>	faces;
};

struct Object
{
	Object() = default;
	~Object() = default;
	Object(const Object& other);
	Object& operator=(const Object& other);

	std::string name;

	Vec3	position;
	Vec4	rotation;

	Mesh		mesh;
	Material	mat;
};
