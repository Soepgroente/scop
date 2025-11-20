#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "VulkanDevice.hpp"

namespace ve {

struct PipelineConfigInfo
{
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};

class VulkanPipeline
{
	public:

	VulkanPipeline(
		VulkanDevice& device,
		const std::string& vertPath,
		const std::string& fragPath,
		const PipelineConfigInfo& PipelineConfigInfo
	);
	~VulkanPipeline();

	VulkanPipeline() = delete;
	VulkanPipeline(const VulkanPipeline&) = delete;
	VulkanPipeline& operator=(const VulkanPipeline&) = delete;

	void	bind(VkCommandBuffer commandBuffer);

	static PipelineConfigInfo	defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:

	static std::vector<char>	readFile(const std::string& filePath);

	void	createGraphicsPipeline(
		const std::string& vertPath,
		const std::string& fragPath,
		const PipelineConfigInfo& pipelineConfigInfo
	);

	void	createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

	VulkanDevice&	vulkanDevice;
	VkPipeline		graphicsPipeline;
	VkShaderModule	vertexShaderModule;
	VkShaderModule	fragmentShaderModule;
};

}