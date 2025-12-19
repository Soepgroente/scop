#include "Quat.hpp"
#include "Vec3.hpp"

quat::quat() noexcept
{
	w = 1.0f;
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

quat::quat(const vec3& v3) noexcept
{
	w = 0.0f;
	x = v3.x;
	y = v3.y;
	z = v3.z;
}

quat::quat(float w, float x, float y, float z) noexcept
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

quat::quat(float w, const vec3& v3) noexcept
{
	this->w = w;
	x = v3.x;
	y = v3.y;
	z = v3.z;
}

quat::quat(const quat& other) noexcept
{
	w = other.w;
	x = other.x;
	y = other.y;
	z = other.z;
}

quat&	quat::operator=(const quat& other) noexcept
{
	if (this != &other)
	{
		w = other.w;
		x = other.x;
		y = other.y;
		z = other.z;
	}
	return *this;
}

quat	quat::clone() const noexcept
{
	return quat(w, x, y, z);
}

quat&	quat::invert() noexcept
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

quat	quat::inverted() const noexcept
{
	return this->clone().invert();
}

quat&	quat::normalize() noexcept
{
	float len = std::sqrt(w * w + x * x + y * y + z * z);

	if (len > 0.0f)
	{
		float invLen = 1.0f / len;

		w *= invLen;
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}
	return *this;
}

quat	quat::normalized() const noexcept
{
	return this->clone().normalize();
}

quat&	quat::fastNormalize() noexcept
{
	float lenSq = w * w + x * x + y * y + z * z;

	if (lenSq > 0.0f)
	{
		float invSqrtLen = fastInverseSqrt(lenSq);

		w *= invSqrtLen;
		x *= invSqrtLen;
		y *= invSqrtLen;
		z *= invSqrtLen;
	}
	return *this;
}

quat	quat::fastNormalized() const noexcept
{
	return this->clone().fastNormalize();
}

quat	quat::product(const quat& a, const quat& b) noexcept
{
	return quat(
		a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
		a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
		a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
		a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w
	);
}

void	quat::angleToVec() noexcept
{
	float angle = w;

	w = std::cos(angle / 2.0f);
	x *= std::sin(angle / 2.0f);
	y *= std::sin(angle / 2.0f);
	z *= std::sin(angle / 2.0f);
}

vec3	quat::rotate(const vec3& rotateAround, quat rotationQuat) noexcept
{
	rotationQuat.angleToVec();
	rotationQuat.invert();
	quat	axis(1.0f, rotateAround);
	quat	resultQuat = quat::product(quat::product(rotationQuat, axis), rotationQuat);

	return vec3(resultQuat.x, resultQuat.y, resultQuat.z);
}