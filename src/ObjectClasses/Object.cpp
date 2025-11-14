#include "Object.hpp"

Object::Object(const Object& other)
{
	*this = other;
}

Object&	Object::operator=(const Object& other)
{
	if (this == &other)
	{
		return *this;
	}
	this->position = other.position;
	this->rotation = other.rotation;
	this->mesh.vertices = other.mesh.vertices;
	this->mesh.normals = other.mesh.normals;
	this->mesh.textureCoords = other.mesh.textureCoords;
	return *this;
}
