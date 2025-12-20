#pragma once

#include "Vectors.hpp"
#include "VulkanWindow.hpp"
#include "VulkanObject.hpp"

namespace ve {

struct MousePos
{
	double	x;
	double	y;
};

class	MouseInput
{
	public:

	void	move(GLFWwindow* window, VulkanObject& object, float deltaTime);

	private:

	float		sensitivity{0.02f};
	MousePos	lastMousePos{0.0, 0.0};
	bool		firstMouseFrame{true};
	int			inputMode{GLFW_CURSOR_NORMAL};
};

} // namespace ve