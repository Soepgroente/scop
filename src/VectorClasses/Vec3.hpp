#pragma once

#include <iostream>

typedef float	v3 __attribute__ ((vector_size(16)));

typedef union	uvec3
{
	v3	data;
	struct
	{
		float x;
		float y;
		float z;
	};
	struct
	{
		float red;
		float green;
		float blue;
	};
}	float3;

class	Vec3
{
	public:

	Vec3() = default;
	~Vec3() = default;
	Vec3(float x, float y, float z);
	Vec3(const v3& other);
	Vec3(const Vec3& other);
	Vec3(const float3& other);
	Vec3&	operator=(const Vec3& vector);

	Vec3	operator+(const Vec3& vectorToAdd)	const;
	Vec3	operator-(const Vec3& vectorToSubtract)	const;
	Vec3	operator*(float scalar)	const;

	bool	operator==(const Vec3& other)	const;
	bool	operator!=(const Vec3& other)	const;

	void	invert() { vec.data *= -1.0f; }
	void	normalize();

	float	length()	const;

	static float	dot(const float3& a, const float3& b);
	static Vec3		cross(const float3& a, const float3& b);
	static Vec3		zero() { return Vec3(Vec3::empty()); }
	static float3	empty() { return float3{0.0f, 0.0f, 0.0f}; }

	float3	vec;

	private:

	static float	hypotenuse(float a, float b) { return std::sqrt(a * a + b * b); }
};

std::ostream&	operator<<(std::ostream& os, const Vec3& vector);