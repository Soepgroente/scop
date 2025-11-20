#include "Scop.hpp"

namespace ve {

Scop::Scop()
{
	createPipelineLayout();
	createPipeline();
	createCommandBuffers();
}

Scop::~Scop()
{
	vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr);
}

void	Scop::run()
{
	while (vulkanWindow.shouldClose() == false)
	{
		glfwPollEvents();
		drawFrame();
	}
}

void	Scop::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo	pipelineLayoutInfo{};

	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	if (vkCreatePipelineLayout(
			vulkanDevice.device(),
			&pipelineLayoutInfo,
			nullptr,
			&pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void	Scop::createPipeline()
{
	PipelineConfigInfo	pipelineConfig = VulkanPipeline::defaultPipelineConfigInfo(
		vulkanSwapChain.getSwapChainExtent().width,
		vulkanSwapChain.getSwapChainExtent().height
	);

	pipelineConfig.pipelineLayout = pipelineLayout;
	pipelineConfig.renderPass = vulkanSwapChain.getRenderPass();

	vulkanPipelinePtr = std::make_unique<VulkanPipeline>(
		vulkanDevice,
		"src/shaders/shadyBusiness.vert.spv",
		"src/shaders/shadyBusiness.frag.spv",
		pipelineConfig
	);
}

void	Scop::createCommandBuffers()
{
	VkCommandBufferAllocateInfo	allocInfo{};

	commandBuffers.resize(vulkanSwapChain.imageCount());

	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = vulkanDevice.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(vulkanDevice.device(),	&allocInfo,	commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo	beginInfo{};

		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo	renderPassInfo{};

		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vulkanSwapChain.getRenderPass();
		renderPassInfo.framebuffer = vulkanSwapChain.getFrameBuffer(i);
		
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = vulkanSwapChain.getSwapChainExtent();

		std::array<VkClearValue, 2>	clearValues{};

		clearValues[0].color = {{0.01f, 0.01f, 0.01f, 1.0f}};
		clearValues[1].depthStencil = {1.0f, 0};
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vulkanPipelinePtr->bind(commandBuffers[i]);
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
		vkCmdEndRenderPass(commandBuffers[i]);
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void	Scop::drawFrame()
{
	uint32_t	imageIndex;
	VkResult result = vulkanSwapChain.acquireNextImage(&imageIndex);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}
	result = vulkanSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}
}

}