#pragma once

#include <cmath>
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Quat.hpp"
#include "Mat3.hpp"
#include "Mat4.hpp"

union FloatIntUnion
{
	float	f;
	int32_t	i;
};

float	fastInverseSqrt(float number) noexcept;
float	degreesToRadians(float degrees) noexcept;
float	radiansToDegrees(float radians) noexcept;