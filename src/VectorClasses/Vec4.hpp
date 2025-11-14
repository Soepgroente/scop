#pragma once

#include <iostream>
#include "Vec3.hpp"

typedef float	v4 __attribute__ ((vector_size(16)));

typedef union	uvec4
{
	v4	data;
	struct
	{
		float x;
		float y;
		float z;
		float w;
	};
	struct
	{
		float real;
		float i;
		float j;
		float k;
	};
	struct
	{
		float r;
		float g;
		float b;
		float opacity;
	};
}	float4;

class	Vec4
{
	public:

	Vec4();
	~Vec4();
	Vec4(float real, float x, float y, float z);
	Vec4(const float4& other);
	Vec4(const Vec4& vector);
	Vec4&	operator=(const Vec4& other);
	float4	vec;

	private:

};

std::ostream&	operator<<(std::ostream& os, const Vec4& vector);