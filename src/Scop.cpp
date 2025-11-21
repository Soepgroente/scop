#include "Scop.hpp"

#include <chrono>
#include <random>

namespace ve {
	
Scop::Scop()
{
	loadModels();
	createPipelineLayout();
	recreateSwapChain();
	createCommandBuffers();
}

static std::vector<VulkanModel::Vertex>	generateSierpinskiTriangle(int depth)
{
	std::vector<VulkanModel::Vertex>	vertices;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);

	glm::vec2	begin[3] =
	{
		{-0.9f, 0.9f},
		{0.9f, 0.9f},
		{0.0f, -0.9f}
	};

	std::function<void(glm::vec2, glm::vec2, glm::vec2, int)>	subdivide =
		[&](glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, int _depth)
	{
		if (_depth == 0)
		{
			vertices.push_back({v1, {dis(gen), dis(gen), dis(gen)}});
			vertices.push_back({v2, {dis(gen), dis(gen), dis(gen)}});
			vertices.push_back({v3, {dis(gen), dis(gen), dis(gen)}});
			return;
		}
		glm::vec2	mid12 = (v1 + v2) / 2.0f;
		glm::vec2	mid23 = (v2 + v3) / 2.0f;
		glm::vec2	mid31 = (v3 + v1) / 2.0f;

		subdivide(v1, mid12, mid31, _depth - 1);
		subdivide(v2, mid23, mid12, _depth - 1);
		subdivide(v3, mid31, mid23, _depth - 1);
	};
	subdivide(begin[0], begin[1], begin[2], depth);
	
	return vertices;
}

Scop::~Scop()
{
	vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr);
}

void	Scop::run()
{
	// std::chrono::steady_clock::time_point	begin, end;
	while (vulkanWindow.shouldClose() == false)
	{
		glfwPollEvents();
		// begin = std::chrono::steady_clock::now();
		drawFrame();
		// end = std::chrono::steady_clock::now();
		// std::chrono::duration<double, std::milli>	elapsed = end - begin;
		// std::cout << "Frame time: " << elapsed.count() << " ms \n";
	}
	vkDeviceWaitIdle(vulkanDevice.device());
}

void	Scop::loadModels()
{
	std::vector<VulkanModel::Vertex>	vertices = generateSierpinskiTriangle(6);

	vulkanModelPtr = std::make_unique<VulkanModel>(vulkanDevice, vertices);
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
	assert(vulkanSwapChainPtr != nullptr && "Cannot create pipeline before swap chain");
	assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

	PipelineConfigInfo	pipelineConfig{};
	
	VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipelineConfig.renderPass = vulkanSwapChainPtr->getRenderPass();

	vulkanPipelinePtr = std::make_unique<VulkanPipeline>(
		vulkanDevice,
		"src/shaders/shadyBusiness.vert.spv",
		"src/shaders/shadyBusiness.frag.spv",
		pipelineConfig
	);
}

void	Scop::recreateSwapChain()
{
	VkExtent2D extent = vulkanWindow.getFramebufferExtent();

	while (extent.width == 0 || extent.height == 0)
	{
		extent = vulkanWindow.getFramebufferExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(vulkanDevice.device());
	if (vulkanSwapChainPtr == nullptr)
	{
		vulkanSwapChainPtr = std::make_unique<VulkanSwapChain>(vulkanDevice, extent);
	}
	else
	{
		vulkanSwapChainPtr = std::make_unique<VulkanSwapChain>(vulkanDevice, extent, std::move(vulkanSwapChainPtr));
		if (vulkanSwapChainPtr->imageCount() != commandBuffers.size())
		{
			freeCommandBuffers();
			createCommandBuffers();
		}
	}

	// if renderpass compatible, don't need to recreate pipeline
	createPipeline();
}

void	Scop::createCommandBuffers()
{
	commandBuffers.resize(vulkanSwapChainPtr->imageCount());
	VkCommandBufferAllocateInfo	allocInfo{};

	commandBuffers.resize(vulkanSwapChainPtr->imageCount());

	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = vulkanDevice.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(vulkanDevice.device(),	&allocInfo,	commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void	Scop::freeCommandBuffers()
{
	vkFreeCommandBuffers(
		vulkanDevice.device(),
		vulkanDevice.getCommandPool(),
		static_cast<uint32_t>(commandBuffers.size()),
		commandBuffers.data()
	);
	commandBuffers.clear();
}

void	Scop::recordCommandBuffer(uint32_t imageIndex)
{
	VkCommandBufferBeginInfo	beginInfo{};

	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}
	VkRenderPassBeginInfo	renderPassInfo{};

	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = vulkanSwapChainPtr->getRenderPass();
	renderPassInfo.framebuffer = vulkanSwapChainPtr->getFrameBuffer(imageIndex);
	
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = vulkanSwapChainPtr->getSwapChainExtent();

	std::array<VkClearValue, 2>	clearValues{};

	clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
	clearValues[1].depthStencil = {1.0f, 0};
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport	viewport{};
	VkRect2D	scissor{{0, 0}, vulkanSwapChainPtr->getSwapChainExtent()};

	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(vulkanSwapChainPtr->getSwapChainExtent().width);
	viewport.height = static_cast<float>(vulkanSwapChainPtr->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

	vulkanPipelinePtr->bind(commandBuffers[imageIndex]);
	vulkanModelPtr->bind(commandBuffers[imageIndex]);
	vulkanModelPtr->draw(commandBuffers[imageIndex]);

	vkCmdDraw(commandBuffers[imageIndex], 3, 1, 0, 0);
	vkCmdEndRenderPass(commandBuffers[imageIndex]);
	if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}

void	Scop::drawFrame()
{
	uint32_t	imageIndex;
	VkResult result = vulkanSwapChainPtr->acquireNextImage(&imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapChain();
		return;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}
	recordCommandBuffer(imageIndex);
	result = vulkanSwapChainPtr->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vulkanWindow.wasWindowResized())
	{
		vulkanWindow.resetWindowResizedFlag();
		recreateSwapChain();
		return;
	}
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}
}

}