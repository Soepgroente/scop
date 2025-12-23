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
	mat4	modelMatrix{1.0f};
	mat4	normalMatrix{1.0f};
};

VulkanRenderSystem::VulkanRenderSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
	: vulkanDevice(device)
{
	createPipelineLayout(globalSetLayout);
	createPipeline(renderPass);
}

VulkanRenderSystem::~VulkanRenderSystem()
{
	vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr);
}

void	VulkanRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
	VkPushConstantRange	pushConstantRange{};

	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	std::vector<VkDescriptorSetLayout>	descriptorSetLayouts = {globalSetLayout};
	VkPipelineLayoutCreateInfo	pipelineLayoutInfo{};

	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(vulkanDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
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

void	VulkanRenderSystem::renderObjects(FrameInfo& frameInfo, std::vector<VulkanObject>& objects, bool rotateModel)
{
	vulkanPipeline->bind(frameInfo.commandBuffer);

	vkCmdBindDescriptorSets(
		frameInfo.commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		pipelineLayout,
		0, 1, &frameInfo.globalDescriptorSet,
		0, nullptr
	);
	for (VulkanObject& object : objects)
	{
		if (rotateModel == true)
		{
			object.transform.rotation.y = std::fmod(object.transform.rotation.y + 0.015f, two_pi());
			// object.transform.rotation.x = mod(object.transform.rotation.x + 0.005f, two_pi<float>());
			// object.transform.rotation.z = mod(object.transform.rotation.z + 0.002f, two_pi<float>());
		}
		SimplePushConstantData	push{};

		push.modelMatrix = object.transform.matrix4();
		push.normalMatrix = object.transform.normalMatrix();

		vkCmdPushConstants(
			frameInfo.commandBuffer,
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push
		);
		object.model->bind(frameInfo.commandBuffer);
		object.model->draw(frameInfo.commandBuffer);
	}
}

} // namespace ve