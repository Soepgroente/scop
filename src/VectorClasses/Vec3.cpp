#include "Vec3.hpp"

Vec3::Vec3(float x, float y, float z)
{
	vec = uvec3{x, y, z};
}

Vec3::Vec3(const v3& other)
{
	this->vec.data = other;
}

Vec3::Vec3(const float3& other)
{
	this->vec.data = other.data;
}

Vec3::Vec3(const Vec3& other)
{
	if (this != &other)
	{
		*this = other;
	}
}

Vec3&	Vec3::operator=(const Vec3& other)
{
	this->vec.data = other.vec.data;
	return *this;
}

Vec3	Vec3::operator*(float scalar)	const
{
	return Vec3(vec.data * scalar);
}

Vec3	Vec3::operator+(const Vec3& vectorToAdd)	const
{
	return Vec3(vec.data + vectorToAdd.vec.data);
}

Vec3	Vec3::operator-(const Vec3& vectorToSubtract)	const
{
	return Vec3(vec.data - vectorToSubtract.vec.data);
}

bool Vec3::operator==(const Vec3& other) const
{
    return vec.x == other.vec.x && 
           vec.y == other.vec.y && 
           vec.z == other.vec.z;
}

bool	Vec3::operator!=(const Vec3& other)	const
{
	return !(*this == other);
}

float	Vec3::length()	const
{
	return Vec3::hypotenuse(Vec3::hypotenuse(vec.x, vec.y), vec.z);
}

void	Vec3::normalize()
{
	vec.data = vec.data / this->length();
}

float	Vec3::dot(const float3& a, const float3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3	Vec3::cross(const float3& a, const float3& b)
{
	return Vec3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

std::ostream&	operator<<(std::ostream& os, const Vec3& vector)
{
	os << "Vec3(" << vector.vec.x << ", " << vector.vec.y << ", " << vector.vec.z << ")";
	return os;
}