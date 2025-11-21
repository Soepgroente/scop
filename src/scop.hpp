#pragma once

#include "VulkanDevice.hpp"
#include "VulkanModel.hpp"
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

	static constexpr int	DEFAULT_WIDTH = 1080;
	static constexpr int	DEFAULT_HEIGHT = 1080;

	private:

	void	loadModels();
	void	createPipelineLayout();
	void	createPipeline();
	void	createCommandBuffers();
	void	freeCommandBuffers();
	void	drawFrame();
	void	recreateSwapChain();
	void	recordCommandBuffer(uint32_t imageIndex);
	
	VulkanWindow	vulkanWindow{DEFAULT_HEIGHT, DEFAULT_WIDTH, "Scop"};
	VulkanDevice	vulkanDevice{vulkanWindow};
	std::unique_ptr<VulkanSwapChain>	vulkanSwapChainPtr;
	
	std::unique_ptr<VulkanPipeline>	vulkanPipelinePtr;
	VkPipelineLayout				pipelineLayout;
	std::vector<VkCommandBuffer>	commandBuffers;
	std::unique_ptr<VulkanModel>	vulkanModelPtr;
};

}