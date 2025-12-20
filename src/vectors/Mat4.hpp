#pragma once

#include <array>
#include <ostream>

class vec3;
class mat3;

class mat4
{
	public:

	mat4();
	mat4(float diagonal);
	mat4(float elements[16]);
	mat4(float m00, float m10, float m20, float m30,
		 float m01, float m11, float m21, float m31,
		 float m02, float m12, float m22, float m32,
		 float m03, float m13, float m23, float m33);
	mat4(std::array<float, 4> col0, std::array<float, 4> col1, std::array<float, 4> col2, std::array<float, 4> col3);
	mat4(const mat3& matrix3x3);
	~mat4() = default;
	mat4(const mat4& other) = default;
	mat4&	operator=(const mat4& other) = default;

	mat4	operator*(const mat4& other) const;
	mat4&	operator*=(const mat4& other);

	mat4&	translate(const vec3& translation) noexcept;
	mat4	translated(const vec3& translation) const noexcept;
	mat4&	rotate(float angleRadians, const vec3& axis) noexcept;
	mat4	rotated(float angleRadians, const vec3& axis) const noexcept;

	union
	{
		float	elements[4][4];
		float	e[16];
	};
	
};

std::ostream&	operator<<(std::ostream& os, const mat4& matrix);