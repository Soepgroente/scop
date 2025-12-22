#include "Mat3.hpp"
#include "Mat4.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"

#include <cmath>
#include <cstring>

mat4::mat4()
{
	std::memset(data, 0, sizeof(data));
}

mat4::mat4(float value)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				data[i][j] = value;
			}
			else
			{
				data[i][j] = 0.0f;
			}
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

mat4::mat4(const vec4& col0,
	const vec4& col1, 
	const vec4& col2,
	const vec4& col3)
{
	for (int i = 0; i < 4; i++)
	{
		data[i][0] = col0[i];
		data[i][1] = col1[i];
		data[i][2] = col2[i];
		data[i][3] = col3[i];
	}
}

// mat4::mat4(const std::array<float, 4>& col0,
// 	const std::array<float, 4>& col1,
// 	const std::array<float, 4>& col2,
// 	const std::array<float, 4>& col3)
// {
// 	for (int i = 0; i < 4; i++)
// 	{
// 		data[i][0] = col0[i];
// 		data[i][1] = col1[i];
// 		data[i][2] = col2[i];
// 		data[i][3] = col3[i];
// 	}
// }

mat4::mat4(std::initializer_list<std::initializer_list<float>> rows)
{
	int	i = 0;
	int	j;

	for (const std::initializer_list<float>& row : rows)
	{
		j = 0;
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

mat4&	mat4::operator*=(const mat4& other)
{
	*this = *this * other;
	return *this;
}

mat4&	mat4::translate(const vec3& translation) noexcept
{
	mat4	translationMatrix(1.0f);

	translationMatrix.data[0][3] = translation.x;
	translationMatrix.data[1][3] = translation.y;
	translationMatrix.data[2][3] = translation.z;
	*this *= translationMatrix;
	return *this;
}

mat4	mat4::translated(const vec3& translation) const noexcept
{
	mat4	translationMatrix(1.0f);

	translationMatrix.data[0][3] = translation.x;
	translationMatrix.data[1][3] = translation.y;
	translationMatrix.data[2][3] = translation.z;
	return (*this) * translationMatrix;
}


mat4&	mat4::rotate(float angleRadians, const vec3& axis) noexcept
{
	const float	cosAngle = std::cos(angleRadians);
	const float	sinAngle = std::sin(angleRadians);
	mat4	r;

	r.data[0][0] = cosAngle + axis.x * axis.x * (1 - cosAngle);
	r.data[0][1] = axis.x * axis.y * (1 - cosAngle) - axis.z * sinAngle;
	r.data[0][2] = axis.x * axis.z * (1 - cosAngle) + axis.y * sinAngle;
	
	r.data[1][0] = axis.y * axis.x * (1 - cosAngle) + axis.z * sinAngle;
	r.data[1][1] = cosAngle + axis.y * axis.y * (1 - cosAngle);
	r.data[1][2] = axis.y * axis.z * (1 - cosAngle) - axis.x * sinAngle;
	
	r.data[2][0] = axis.z * axis.x * (1 - cosAngle) - axis.y * sinAngle;
	r.data[2][1] = axis.z * axis.y * (1 - cosAngle) + axis.x * sinAngle;
	r.data[2][2] = cosAngle + axis.z * axis.z * (1 - cosAngle);
	
	r.data[0][3] = 0.0f;
	r.data[1][3] = 0.0f;
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