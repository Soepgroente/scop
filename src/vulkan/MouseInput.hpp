#pragma once

#include "VulkanWindow.hpp"
#include "VulkanObject.hpp"

namespace ve {

class	MouseInput
{
	public:

	void	move(GLFWwindow* window, VulkanObject& object, float deltaTime);

	private:

	float	sensitivity{0.02f};
	glm::dvec2	lastMousePos{0.0, 0.0};
	bool	firstMouseFrame{true};
	int		inputMode{GLFW_CURSOR_NORMAL};
};

} // namespace ve