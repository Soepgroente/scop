#pragma once

#include <array>
#include <ostream>

class mat3
{
	public:

	mat3() = default;
	mat3(float diagonal);
	mat3(float elements[9]);
	mat3(std::array<float, 3> col0,
		 std::array<float, 3> col1,
		 std::array<float, 3> col2);
	~mat3() = default;
	mat3(const mat3& other) = default;
	mat3&	operator=(const mat3& other) = default;

	mat3	operator*(const mat3& other) const;
	mat3&	operator*=(const mat3& other) {	*this = *this * other; return *this;}

	float*			operator[](int row) noexcept { return elements[row]; }
	const float*	operator[](int row) const noexcept { return elements[row]; }

	union 
	{
		float e[9];
		float elements[3][3];
	};
	
};

std::ostream&	operator<<(std::ostream& os, const mat3& matrix);