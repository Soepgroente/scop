#pragma once

#include "VulkanObject.hpp"
#include "VulkanModel.hpp"

#include <GLFW/glfw3.h>

namespace ve {

class KeyboardInput
{
	public:

	struct KeyMappings
	{
		int moveLeft = GLFW_KEY_A;
		int moveRight = GLFW_KEY_D;
		int moveForward = GLFW_KEY_W;
		int moveBackward = GLFW_KEY_S;
		int moveUp = GLFW_KEY_E;
		int moveDown = GLFW_KEY_Q;
		int lookLeft = GLFW_KEY_LEFT;
		int lookRight = GLFW_KEY_RIGHT;
		int lookUp = GLFW_KEY_UP;
		int lookDown = GLFW_KEY_DOWN;
		int rotate = GLFW_KEY_KP_ADD;
	};

	void	move(GLFWwindow* window, VulkanObject& object, float deltaTime);

	KeyMappings	keys{};

	float	movementSpeed{10.0f};
	float	lookSpeed{1.0f};
};

} // namespace ve