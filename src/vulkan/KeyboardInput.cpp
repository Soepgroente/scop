#include "KeyboardInput.hpp"

namespace ve {

void	KeyboardInput::move(GLFWwindow* window, VulkanObject& object, float deltaTime)
{
	glm::vec3	rotate{0.0f};

	if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
	if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;
	if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
	if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;

	if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
	{
		object.transform.rotation += glm::normalize(rotate) * lookSpeed * deltaTime;
	}

	object.transform.rotation.x = glm::clamp(object.transform.rotation.x, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);
	object.transform.rotation.y = glm::mod(object.transform.rotation.y, glm::two_pi<float>());

	float	rot = object.transform.rotation.y;
	const glm::vec3	forwardDir{glm::sin(rot), 0.0f, glm::cos(rot)};
	const glm::vec3	rightDir{forwardDir.z, 0.0f, -forwardDir.x};
	const glm::vec3	upDir{0.0f, -1.0f, 0.0f};

	glm::vec3	moveDir{0.0f};
	if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
	if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
	if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
	if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
	if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
	if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

	if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
	{
		object.transform.translation += glm::normalize(moveDir) * movementSpeed * deltaTime;
	}
}

} // namespace ve