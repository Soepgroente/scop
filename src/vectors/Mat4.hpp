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
	mat4(const std::array<float, 4>& row0,
		 const std::array<float, 4>& row1,
		 const std::array<float, 4>& row2,
		 const std::array<float, 4>& row3);
	mat4(std::initializer_list<std::initializer_list<float>> rows);
	mat4(const mat3& matrix3x3);
	~mat4() = default;
	mat4(const mat4& other);
	mat4&	operator=(const mat4& other);

	mat4	operator*(const mat4& other) const;
	mat4&	operator*=(const mat4& other);

	mat4&	translate(const vec3& translation) noexcept;
	mat4	translated(const vec3& translation) const noexcept;
	mat4&	rotate(float angleRadians, const vec3& axis) noexcept;
	mat4	rotated(float angleRadians, const vec3& axis) const noexcept;

	float	data[4][4];
	
};

std::ostream&	operator<<(std::ostream& os, const mat4& matrix);