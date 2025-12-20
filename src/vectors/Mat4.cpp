#include "Mat3.hpp"
#include "Mat4.hpp"
#include "Vec3.hpp"

#include <cmath>
#include <cstring>

/* mat4::mat4() : elements{{0.0f}, {0.0f}, {0.0f}, {0.0f}} {}

mat4::mat4(float diagonal) : elements{{0.0f}, {0.0f}, {0.0f}, {0.0f}}
{
	elements[0][0] = diagonal;
	elements[1][1] = diagonal;
	elements[2][2] = diagonal;
	elements[3][3] = diagonal;
}

mat4::mat4(float elements[16])
{
	std::memcpy(this->elements, elements, 16 * sizeof(float));
}

mat4::mat4(float m00, float m10, float m20, float m30,
			float m01, float m11, float m21, float m31,
			float m02, float m12, float m22, float m32,
			float m03, float m13, float m23, float m33)
{
	elements[0][0] = m00; elements[1][0] = m10; elements[2][0] = m20; elements[3][0] = m30;
	elements[0][1] = m01; elements[1][1] = m11; elements[2][1] = m21; elements[3][1] = m31;
	elements[0][2] = m02; elements[1][2] = m12; elements[2][2] = m22; elements[3][2] = m32;
	elements[0][3] = m03; elements[1][3] = m13; elements[2][3] = m23; elements[3][3] = m33;
}

mat4::mat4(std::array<float, 4> col0, std::array<float, 4> col1, std::array<float, 4> col2, std::array<float, 4> col3)
{
	// column 0
	elements[0][0] = col0[0]; 
	elements[1][0] = col0[1];
	elements[2][0] = col0[2];
	elements[3][0] = col0[3];

	// column 1
	elements[0][1] = col1[0];
	elements[1][1] = col1[1];
	elements[2][1] = col1[2];
	elements[3][1] = col1[3];

	// column 2
	elements[0][2] = col2[0];
	elements[1][2] = col2[1];
	elements[2][2] = col2[2];
	elements[3][2] = col2[3];

	// column 3
	elements[0][3] = col3[0];
	elements[1][3] = col3[1];
	elements[2][3] = col3[2];
	elements[3][3] = col3[3];
}

mat4::mat4(const mat3& matrix3x3)
{
	elements[0][0] = matrix3x3.elements[0][0]; elements[1][0] = matrix3x3.elements[1][0]; elements[2][0] = matrix3x3.elements[2][0]; elements[3][0] = 0.0f;
	elements[0][1] = matrix3x3.elements[0][1]; elements[1][1] = matrix3x3.elements[1][1]; elements[2][1] = matrix3x3.elements[2][1]; elements[3][1] = 0.0f;
	elements[0][2] = matrix3x3.elements[0][2]; elements[1][2] = matrix3x3.elements[1][2]; elements[2][2] = matrix3x3.elements[2][2]; elements[3][2] = 0.0f;
	elements[0][3] = 0.0f;                     elements[1][3] = 0.0f;                     elements[2][3] = 0.0f;                     elements[3][3] = 1.0f;
}

mat4	mat4::operator*(const mat4& other) const
{
	mat4	result;

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			float	sum = 0.0f;

			for (int i = 0; i < 4; i++)
			{
				sum += this->elements[row][i] * other.elements[i][col];
			}
			result.elements[row][col] = sum;
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
	elements[0][3] += translation.x;
	elements[1][3] += translation.y;
	elements[2][3] += translation.z;
	return *this;
}

mat4	mat4::translated(const vec3& translation) const noexcept
{
	mat4	result = *this;

	result.translate(translation);
	return result;
} */

/*	Assumes axis is already normalized	*/


mat4&	mat4::rotate(float angleRadians, const vec3& axis) noexcept
{
	float	cosAngle = std::cos(angleRadians);
	float	sinAngle = std::sin(angleRadians);
	mat4	r;

	r.data[0][0] = cosAngle + axis.x * axis.x * (1 - cosAngle);
	r.data[0][1] = axis.x * axis.y * (1 - cosAngle) - axis.z * sinAngle;
	r.data[0][2] = axis.x * axis.z * (1 - cosAngle) + axis.y * sinAngle;
	r.data[0][3] = 0.0f;

	r.data[1][0] = axis.y * axis.x * (1 - cosAngle) + axis.z * sinAngle;
	r.data[1][1] = cosAngle + axis.y * axis.y * (1 - cosAngle);
	r.data[1][2] = axis.y * axis.z * (1 - cosAngle) - axis.x * sinAngle;
	r.data[1][3] = 0.0f;

	r.data[2][0] = axis.z * axis.x * (1 - cosAngle) - axis.y * sinAngle;
	r.data[2][1] = axis.z * axis.y * (1 - cosAngle) + axis.x * sinAngle;
	r.data[2][2] = cosAngle + axis.z * axis.z * (1 - cosAngle);
	r.data[2][3] = 0.0f;

	r.data[3][0] = 0.0f;
	r.data[3][1] = 0.0f;
	r.data[3][2] = 0.0f;
	r.data[3][3] = 1.0f;

	*this *= r;
	return *this;
}

mat4	mat4::rotated(float angleRadians, const vec3& axis) const noexcept
{
	mat4	result = *this;

	result.rotate(angleRadians, axis);
	return result;
}


// Default constructor - creates identity matrix
mat4::mat4()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			data[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}
}

// Constructor from single value - creates identity matrix scaled by value
mat4::mat4(float value)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			data[i][j] = (i == j) ? value : 0.0f;
		}
	}
}

mat4::mat4(const mat3& matrix3x3)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			data[i][j] = matrix3x3.elements[i][j];
		}
	}
	data[0][3] = 0.0f;
	data[1][3] = 0.0f;
	data[2][3] = 0.0f;
	data[3][0] = 0.0f;
	data[3][1] = 0.0f;
	data[3][2] = 0.0f;
	data[3][3] = 1.0f;
}

mat4::mat4(const std::array<float, 4>& row0,
	const std::array<float, 4>& row1, 
	const std::array<float, 4>& row2,
	const std::array<float, 4>& row3)
{
	std::copy(row0.begin(), row0.end(), data[0]);
	std::copy(row1.begin(), row1.end(), data[1]);
	std::copy(row2.begin(), row2.end(), data[2]);
	std::copy(row3.begin(), row3.end(), data[3]);
}

mat4::mat4(std::initializer_list<std::initializer_list<float>> rows)
{
	int i = 0;

	for (const std::initializer_list<float>& row : rows)
	{
		int j = 0;
		for (float val : row)
		{
			if (i < 4 && j < 4)
			{
				data[i][j] = val;
			}
			j++;
		}
		i++;
	}
}

mat4::mat4(const mat4& other)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			data[i][j] = other.data[i][j];
		}
	}
}

mat4&	mat4::operator=(const mat4& other)
{
	if (this != &other)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				data[i][j] = other.data[i][j];
			}
		}
	}
	return *this;
}

mat4&	mat4::operator*=(const mat4& other)
{
	*this = *this * other;
	return *this;
}

mat4&	mat4::translate(const vec3& translation) noexcept
{
	data[0][3] += translation.x;
	data[1][3] += translation.y;
	data[2][3] += translation.z;
	return *this;
}

mat4	mat4::translated(const vec3& translation) const noexcept
{
	mat4 result = *this;

	result.translate(translation);
	return result;
}

mat4	mat4::operator*(const mat4& other) const
{
	mat4	result(0.0f);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.data[i][j] += data[i][k] * other.data[k][j];
			}
		}
	}
	return result;
}



// Access operators
// float* operator[](int row)
// {
// 	return data[row];
// }

// const float* operator[](int row) const
// {
// 	return data[row];
// }

// mat4	mat4::rotated(float angleRadians, const vec3& axis) const noexcept
// {
// 	mat4	result = *this;

// 	result.rotate(angleRadians, axis);
// 	return result;
// }

std::ostream&	operator<<(std::ostream& os, const mat4& matrix)
{
	for (int row = 0; row < 4; row++)
	{
		os << "[";
		for (int col = 0; col < 4; col++)
		{
			os << matrix.data[row][col] << "]";
			if (col < 3)
			{
				os << " [";
			}
		}
		os << "\n";
	}
	return os;
}