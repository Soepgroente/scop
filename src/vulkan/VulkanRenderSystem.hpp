#pragma once

#include "Camera.hpp"
#include "VulkanDevice.hpp"
#include "VulkanModel.hpp"
#include "VulkanObject.hpp"
#include "VulkanPipeline.hpp"

#include <GLFW/glfw3.h>
#include <array>
#include <memory>
#include <vector>

namespace ve {

class VulkanRenderSystem
{
	public:

	VulkanRenderSystem(VulkanDevice& device, VkRenderPass renderPass);
	~VulkanRenderSystem();
	
	VulkanRenderSystem() = delete;
	VulkanRenderSystem(const VulkanRenderSystem&) = delete;
	VulkanRenderSystem& operator=(const VulkanRenderSystem&) = delete;
	
	void	renderObjects(VkCommandBuffer commandBuffer, std::vector<VulkanObject>& objects, const Camera& camera);

	private:

	void	createPipelineLayout();
	void	createPipeline(VkRenderPass renderPass);

	VulkanDevice&	vulkanDevice;

	std::unique_ptr<VulkanPipeline>		vulkanPipeline;
	VkPipelineLayout					pipelineLayout;
};

}