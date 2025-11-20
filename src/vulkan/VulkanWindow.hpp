#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>


namespace ve {

class VulkanWindow
{
	public:

	VulkanWindow() = delete;
	VulkanWindow(int width, int height, const char* title);
	VulkanWindow(const VulkanWindow&) = delete;
	VulkanWindow& operator=(const VulkanWindow&) = delete;
	~VulkanWindow();

	bool	shouldClose() const { return glfwWindowShouldClose(window); }

	void	createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	VkExtent2D	getFramebufferExtent() const { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
	private:

	void	initWindow();

	const int	width;
	const int	height;

	std::string	title;
	GLFWwindow*	window;
};

}