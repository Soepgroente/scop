#include "Mat3.hpp"

mat3::mat3(float diagonal) : elements{0.0f}
{
	elements[0] = diagonal;
	elements[1 + 1 * 3] = diagonal;
	elements[2 + 2 * 3] = diagonal;
}

mat3::mat3(float elements[3 * 3])
{
	for (int i = 0; i < 9; i++)
	{
		this->elements[i] = elements[i];
	}
}

mat3::mat3(float row0[3], float row1[3], float row2[3])
{
	for (int i = 0; i < 3; i++)
	{
		elements[i + 0 * 3] = row0[i];
		elements[i + 1 * 3] = row1[i];
		elements[i + 2 * 3] = row2[i];
	}
}

mat3	mat3::operator*(const mat3& other) const
{
	mat3	result;

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			float	sum = 0.0f;

			for (int e = 0; e < 3; e++)
			{
				sum += this->at(row, e) * other.at(e, col);
			}
			result.at(row, col) = sum;
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
			os << matrix.at(row, col) << "]";
			if (col < 2)
			{
				os << " [";
			}
		}
		os << "\n";
	}
	return os;
}