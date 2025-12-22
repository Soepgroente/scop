#include "Mat3.hpp"
#include <cstring>

mat3::mat3(float diagonal) : elements{{0.0f}, {0.0f}, {0.0f}}
{
	elements[0][0] = diagonal;
	elements[1][1] = diagonal;
	elements[2][2] = diagonal;
}

mat3::mat3(float elements[9])
{
	std::memcpy(this->e, elements, 9 * sizeof(float));
}

mat3::mat3(std::array<float, 3> col0, std::array<float, 3> col1, std::array<float, 3> col2)
{
	for (int i = 0; i < 3; i++)
	{
		elements[i][0] = col0[i];
		elements[i][1] = col1[i];
		elements[i][2] = col2[i];
	}
}

mat3	mat3::operator*(const mat3& other) const
{
	mat3	result(0.0f);
	
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			result.elements[row][col] = 
				elements[row][0] * other.elements[0][col] +
				elements[row][1] * other.elements[1][col] +
				elements[row][2] * other.elements[2][col];
		}
	}
	return result;
}

std::ostream&	operator<<(std::ostream& os, const mat3& matrix)
{
	for (int row = 0; row < 3; row++)
	{
		os << "[";
		for (int col = 0; col < 3; col++)
		{
			os << matrix.elements[row][col] << "]";
			if (col < 2)
			{
				os << " [";
			}
		}
		os << "\n";
	}
	return os;
}