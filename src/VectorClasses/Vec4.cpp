#include "Vec4.hpp"

Vec4::Vec4()
{
}

Vec4::~Vec4()
{
}

Vec4::Vec4(float x, float y, float z, float w)
{
	vec = float4{x, y, z, w};
}

Vec4::Vec4(const float4& other)
{
	vec = other;
}

Vec4::Vec4(const Vec4& other)
{
	if (this != &other)
	{
		*this = other;
	}
}

Vec4&	Vec4::operator=(const Vec4& other)
{
	this->vec = other.vec;
	return *this;
}

std::ostream&	operator<<(std::ostream& os, const Vec4& vector)
{
	os << "Vec4(" << vector.vec.real << ", " << vector.vec.x << ", " << vector.vec.y << ", " << vector.vec.z << ")";
	return os;
}