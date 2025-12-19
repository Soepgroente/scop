#include "Mat4.hpp"
#include "Vec3.hpp"

mat4::mat4() : elements{0.0f} {}

mat4::mat4(float diagonal) : elements{0.0f}
{
	elements[0] = diagonal;
	elements[1 + 1 * 4] = diagonal;
	elements[2 + 2 * 4] = diagonal;
	elements[3 + 3 * 4] = diagonal;
}

mat4::mat4(float elements[4 * 4])
{
	for (int i = 0; i < 16; i++)
	{
		this->elements[i] = elements[i];
	}
}

mat4::mat4(float col0[4], float col1[4], float col2[4], float col3[4])
{
	for (int row = 0; row < 4; row++)
	{
		elements[0 + row * 4] = col0[row];
		elements[1 + row * 4] = col1[row];
		elements[2 + row * 4] = col2[row];
		elements[3 + row * 4] = col3[row];
	}
}

mat4::mat4(const mat4& other)
{
	for (int i = 0; i < 16; i++)
	{
		elements[i] = other.elements[i];
	}
}

mat4&	mat4::operator=(const mat4& other)
{
	if (this != &other)
	{
		for (int i = 0; i < 16; i++)
		{
			elements[i] = other.elements[i];
		}
	}
	return *this;
}

float&	mat4::operator[](int index) noexcept
{
	return elements[index];
}

const float&	mat4::operator[](int index) const noexcept
{
	return elements[index];
}

float&	mat4::at(int row, int col) noexcept
{
	return elements[col + row * 4];
}

const float&	mat4::at(int row, int col) const noexcept
{
	return elements[col + row * 4];
}

mat4	mat4::operator*(const mat4& other) const
{
	mat4	result;

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			float	sum = 0.0f;

			for (int e = 0; e < 4; e++)
			{
				sum += this->at(row, e) * other.at(e, col);
			}
			result.at(row, col) = sum;
		}
	}
	return result;
}

mat4&	mat4::operator*=(const mat4& other)
{
	*this = *this * other;
	return *this;
}

mat4&	mat4::translate(const vec3& translation) noexcept
{
	elements[3 + 0 * 4] += translation.x;
	elements[3 + 1 * 4] += translation.y;
	elements[3 + 2 * 4] += translation.z;
	return *this;
}

mat4	mat4::translated(const vec3& translation) const noexcept
{
	mat4	result = *this;

	result.elements[3 + 0 * 4] += translation.x;
	result.elements[3 + 1 * 4] += translation.y;
	result.elements[3 + 2 * 4] += translation.z;
	return result;
}

mat4&	mat4::rotate(float angleRadians, const vec3& axis) noexcept
{
	const float	c = cosf(angleRadians);
	const float	s = sinf(angleRadians);
	const float	oneMinusC = 1.0f - c;

	at(0, 0) = axis.x * axis.x * oneMinusC + c;
	at(0, 1) = axis.x * axis.y * oneMinusC - axis.z * s;
	at(0, 2) = axis.x * axis.z * oneMinusC + axis.y * s;

	at(1, 0) = axis.y * axis.x * oneMinusC + axis.z * s;
	at(1, 1) = axis.y * axis.y * oneMinusC + c;
	at(1, 2) = axis.y * axis.z * oneMinusC - axis.x * s;

	at(2, 0) = axis.z * axis.x * oneMinusC - axis.y * s;
	at(2, 1) = axis.z * axis.y * oneMinusC + axis.x * s;
	at(2, 2) = axis.z * axis.z * oneMinusC + c;

	return *this;
}

mat4	mat4::rotated(float angleRadians, const vec3& axis) const noexcept
{
	mat4	result = *this;

	result.rotate(angleRadians, axis);
	return result;
}

std::ostream&	operator<<(std::ostream& os, const mat4& matrix)
{
	for (int row = 0; row < 4; row++)
	{
		os << "[";
		for (int col = 0; col < 4; col++)
		{
			os << matrix.at(row, col) << "]";
			if (col < 3)
			{
				os << " [";
			}
		}
		os << "\n";
	}
	return os;
}