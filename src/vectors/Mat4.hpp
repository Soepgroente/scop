#pragma once

#include <ostream>

class vec3;

class mat4
{
	public:

	mat4();
	mat4(float diagonal);
	mat4(float elements[4 * 4]);
	mat4(float col0[4], float col1[4], float col2[4], float col3[4]);
	~mat4() = default;
	mat4(const mat4& other);
	mat4&	operator=(const mat4& other);

	float&			operator[](int index) noexcept;
	const float&	operator[](int index) const noexcept;
	float&			at(int row, int col) noexcept;
	const float&	at(int row, int col) const noexcept;

	mat4	operator*(const mat4& other) const;
	mat4&	operator*=(const mat4& other);

	mat4&	translate(const vec3& translation) noexcept;
	mat4	translated(const vec3& translation) const noexcept;
	mat4&	rotate(float angleRadians, const vec3& axis) noexcept;
	mat4	rotated(float angleRadians, const vec3& axis) const noexcept;

	float	elements[4 * 4];
};

std::ostream&	operator<<(std::ostream& os, const mat4& matrix);