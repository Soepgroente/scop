#pragma once

#include <ostream>

class mat3
{
	public:

	mat3() = default;
	mat3(float diagonal);
	mat3(float elements[3 * 3]);
	mat3(float row0[3], float row1[3], float row2[3]);
	~mat3() = default;
	mat3(const mat3& other) = default;
	mat3&	operator=(const mat3& other) = default;

	float&			operator[](int index) { return elements[index]; }
	const float&	operator[](int index) const { return elements[index]; }
	float&			at(int row, int col) { return elements[col + row * 3]; }
	const float&	at(int row, int col) const { return elements[col + row * 3]; }

	mat3	operator*(const mat3& other) const;
	mat3&	operator*=(const mat3& other) {	*this = *this * other; return *this;}

	float	elements[3 * 3];
};

std::ostream&	operator<<(std::ostream& os, const mat3& matrix);