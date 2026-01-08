#include "KeyboardInput.hpp"
#include "Vectors.hpp"

namespace ve {

KeyboardInput::KeyMappings	KeyboardInput::keys{};

KeyboardInput::KeyboardInput(GLFWwindow* window) : window{window}
{
}

bool	KeyboardInput::shouldRotate(size_t& lastPressed, size_t frameCount)
{
	if (frameCount - lastPressed > 20 && glfwGetKey(window, keys.rotate) == GLFW_PRESS)
	{
		lastPressed = frameCount;
		return true;
	}
	return false;
}

void	KeyboardInput::move(VulkanObject& object, float deltaTime)
{
	vec3	rotation{0.0f};

	if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotation.y += 1.0f;
	if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotation.y -= 1.0f;
	if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotation.x -= 1.0f;
	if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotation.x += 1.0f;

	if (vec3::dot(rotation, rotation) > std::numeric_limits<float>::epsilon())
	{
		object.transform.rotation += rotation.normalize() * lookSpeed * deltaTime;
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

void	KeyboardInput::registerKeyPresses()
{
	keys.moveLeft = glfwGetKey(window, keys.moveLeft);
	keys.moveRight = glfwGetKey(window, keys.moveRight);
	keys.moveForward = glfwGetKey(window, keys.moveForward);
	keys.moveBackward = glfwGetKey(window, keys.moveBackward);
	keys.moveUp = glfwGetKey(window, keys.moveUp);
	keys.moveDown = glfwGetKey(window, keys.moveDown);
	keys.lookLeft = glfwGetKey(window, keys.lookLeft);
	keys.lookRight = glfwGetKey(window, keys.lookRight);
	keys.lookUp = glfwGetKey(window, keys.lookUp);
	keys.lookDown = glfwGetKey(window, keys.lookDown);
	keys.rotate = glfwGetKey(window, keys.rotate);
	keys.toggleTexture = glfwGetKey(window, keys.toggleTexture);
}

void	KeyboardInput::resetRegisteredKeys()
{
	std::memset(&keys, GLFW_RELEASE, sizeof(keys));
}

} // namespace ve