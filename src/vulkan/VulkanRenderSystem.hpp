#pragma once

#include "Camera.hpp"
#include "VulkanDevice.hpp"
#include "VulkanFrameInfo.hpp"
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
	
	void	renderObjects(FrameInfo& info, std::vector<VulkanObject>& objects, bool rotateModel);

	private:

	void	createPipelineLayout();
	void	createPipeline(VkRenderPass renderPass);

	VulkanDevice&	vulkanDevice;

	std::unique_ptr<VulkanPipeline>		vulkanPipeline;
	VkPipelineLayout					pipelineLayout;
};

}