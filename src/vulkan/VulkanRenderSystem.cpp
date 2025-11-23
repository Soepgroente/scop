#include "VulkanRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>
#include <random>

namespace ve {

struct SimplePushConstantData
{
	glm::mat2	transform{1.0f, 0.0f, 0.0f, 1.0f};
	glm::vec2	offset;
	alignas(16) glm::vec3	color;
};

VulkanRenderSystem::VulkanRenderSystem(VulkanDevice& device, VkRenderPass renderPass)
	: vulkanDevice(device)
{
	createPipelineLayout();
	createPipeline(renderPass);
}

VulkanRenderSystem::~VulkanRenderSystem()
{
	vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr);
}

void	VulkanRenderSystem::createPipelineLayout()
{
	VkPushConstantRange	pushConstantRange{};

	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	VkPipelineLayoutCreateInfo	pipelineLayoutInfo{};

	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(
			vulkanDevice.device(),
			&pipelineLayoutInfo,
			nullptr,
			&pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void	VulkanRenderSystem::createPipeline(VkRenderPass renderPass)
{
	assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

	PipelineConfigInfo	pipelineConfig{};
	
	VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipelineConfig.renderPass = renderPass;

	vulkanPipeline = std::make_unique<VulkanPipeline>(
		vulkanDevice,
		"src/shaders/shadyBusiness.vert.spv",
		"src/shaders/shadyBusiness.frag.spv",
		pipelineConfig
	);
}

void	VulkanRenderSystem::renderObjects(VkCommandBuffer commandBuffer, std::vector<VulkanObject>& objects)
{
	vulkanPipeline->bind(commandBuffer);

	for (VulkanObject& object : objects)
	{
		object.transform2d.rotation = glm::mod(
			object.transform2d.rotation + 0.0005f * glm::two_pi<float>(),
			glm::two_pi<float>()
		);
		SimplePushConstantData	push{};

		push.offset = object.transform2d.translation;
		push.color = object.color;
		push.transform = object.transform2d.mat2();

		vkCmdPushConstants(
			commandBuffer,
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push
		);
		object.model->bind(commandBuffer);
		object.model->draw(commandBuffer);
	}
}

}