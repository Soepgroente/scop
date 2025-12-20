#include "Camera.hpp"
#include "Vectors.hpp"

#include <cassert>

namespace ve {

void	Camera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far)
{
	projectionMatrix = mat4{1.0f};
	projectionMatrix.elements[0][0] = 2.f / (right - left);
	projectionMatrix.elements[1][1] = 2.f / (bottom - top);
	projectionMatrix.elements[2][2] = 1.f / (far - near);
	projectionMatrix.elements[3][0] = -(right + left) / (right - left);
	projectionMatrix.elements[3][1] = -(bottom + top) / (bottom - top);
	projectionMatrix.elements[3][2] = -near / (far - near);
}

void	Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far)
{
	assert(std::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
	const float tanHalfFovy = tan(fovy / 2.f);

	projectionMatrix = mat4{0.0f};
	projectionMatrix.elements[0][0] = 1.f / (aspect * tanHalfFovy);
	projectionMatrix.elements[1][1] = -1.f / tanHalfFovy;
	projectionMatrix.elements[2][2] = far / (far - near);
	projectionMatrix.elements[2][3] = 1.f;
	projectionMatrix.elements[3][2] = -(far * near) / (far - near);
}

void	Camera::setViewDirection(vec3 position, vec3 direction, vec3 up)
{
	const vec3 w = direction.normalized();
	const vec3 u = vec3::cross(w, up).normalized();
	const vec3 v = vec3::cross(u, w);

	viewMatrix = mat4{1.f};
	viewMatrix.elements[0][0] = u.x;
	viewMatrix.elements[1][0] = u.y;
	viewMatrix.elements[2][0] = u.z;
	viewMatrix.elements[0][1] = v.x;
	viewMatrix.elements[1][1] = v.y;
	viewMatrix.elements[2][1] = v.z;
	viewMatrix.elements[0][2] = w.x;
	viewMatrix.elements[1][2] = w.y;
	viewMatrix.elements[2][2] = w.z;
	viewMatrix.elements[3][0] = -vec3::dot(u, position);
	viewMatrix.elements[3][1] = -vec3::dot(v, position);
	viewMatrix.elements[3][2] = -vec3::dot(w, position);
}

void	Camera::setViewTarget(vec3 position, vec3 target, vec3 up)
{
	setViewDirection(position, target - position, up);
}

void	Camera::setViewYXZ(vec3 position, vec3 rotation)
{
	const float c3 = std::cos(rotation.z);
	const float s3 = std::sin(rotation.z);
	const float c2 = std::cos(rotation.x);
	const float s2 = std::sin(rotation.x);
	const float c1 = std::cos(rotation.y);
	const float s1 = std::sin(rotation.y);

	const vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
	const vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
	const vec3 w{(c2 * s1), (-s2), (c1 * c2)};

	viewMatrix = mat4{1.f};
	viewMatrix.elements[0][0] = u.x;
	viewMatrix.elements[1][0] = u.y;
	viewMatrix.elements[2][0] = u.z;
	viewMatrix.elements[0][1] = v.x;
	viewMatrix.elements[1][1] = v.y;
	viewMatrix.elements[2][1] = v.z;
	viewMatrix.elements[0][2] = w.x;
	viewMatrix.elements[1][2] = w.y;
	viewMatrix.elements[2][2] = w.z;
	viewMatrix.elements[3][0] = -vec3::dot(u, position);
	viewMatrix.elements[3][1] = -vec3::dot(v, position);
	viewMatrix.elements[3][2] = -vec3::dot(w, position);
}

}	// namespace ve