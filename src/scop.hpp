#pragma once

#include "VulkanDevice.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanWindow.hpp"

#include <GLFW/glfw3.h>
#include <array>
#include <memory>
#include <vector>

namespace ve {

class Scop
{
	public:

	Scop();
	~Scop();
	
	Scop(const Scop&) = delete;
	Scop& operator=(const Scop&) = delete;

	void	run();

	static constexpr int	DEFAULT_WIDTH = 1440;
	static constexpr int	DEFAULT_HEIGHT = 1440;

	private:
	
	VulkanWindow	vulkanWindow{DEFAULT_HEIGHT, DEFAULT_WIDTH, "Scop"};
	VulkanDevice	vulkanDevice{vulkanWindow};
	VulkanSwapChain	vulkanSwapChain{vulkanDevice, vulkanWindow.getFramebufferExtent()};
	
	std::unique_ptr<VulkanPipeline>	vulkanPipelinePtr;
	VkPipelineLayout				pipelineLayout;
	std::vector<VkCommandBuffer>	commandBuffers;

	void	createPipelineLayout();
	void	createPipeline();
	void	createCommandBuffers();
	void	drawFrame();
};

}