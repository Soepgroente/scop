#include "KeyboardInput.hpp"
#include "Vectors.hpp"

namespace ve {

void	KeyboardInput::move(GLFWwindow* window, VulkanObject& object, float deltaTime)
{
	vec3	rotate{0.0f};

	if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
	if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;
	if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x -= 1.0f;
	if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x += 1.0f;

	if (vec3::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
	{
		object.transform.rotation += rotate.normalize() * lookSpeed * deltaTime;
	}

	object.transform.rotation.x = std::clamp(object.transform.rotation.x, -half_pi() + 0.01f, half_pi() - 0.01f);
	object.transform.rotation.y = std::fmod(object.transform.rotation.y, two_pi());

	float	rot = object.transform.rotation.y;

	const vec3	forwardDir{std::sin(rot), 0.0f, std::cos(rot)};
	const vec3	rightDir{forwardDir.z, 0.0f, -forwardDir.x};
	const vec3	upDir{0.0f, -1.0f, 0.0f};

	vec3	moveDir{0.0f};

	if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
	if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
	if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
	if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
	if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir -= upDir;
	if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir += upDir;

	if (vec3::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
	{
		object.transform.translation += moveDir.normalize() * movementSpeed * deltaTime;
	}
}

} // namespace ve