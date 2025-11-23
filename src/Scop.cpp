#include "Scop.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>
#include <random>

namespace ve {

Scop::Scop(std::string objPath) : objModelPath(objPath)
{
	loadObjects();
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
}

void	Scop::run()
{
	VulkanRenderSystem	renderSystem{vulkanDevice, vulkanRenderer.getSwapChainRenderPass()};

	// std::chrono::steady_clock::time_point	begin, end;
	while (vulkanWindow.shouldClose() == false)
	{
		glfwPollEvents();

		VkCommandBuffer commandBuffer = vulkanRenderer.beginFrame();
		if (commandBuffer != nullptr)
		{
			vulkanRenderer.beginSwapChainRenderPass(commandBuffer);
			renderSystem.renderObjects(commandBuffer, objects);
			vulkanRenderer.endSwapChainRenderPass(commandBuffer);
			vulkanRenderer.endFrame();
		}
		// begin = std::chrono::steady_clock::now();
		// end = std::chrono::steady_clock::now();
		// std::chrono::duration<double, std::milli>	elapsed = end - begin;
		// std::cout << "Frame time: " << elapsed.count() << " ms \n";
	}
	vkDeviceWaitIdle(vulkanDevice.device());
}

void	Scop::loadObjects()
{
	std::vector<VulkanModel::Vertex>	vertices = generateSierpinskiTriangle(6);

	std::shared_ptr<VulkanModel> model = std::make_shared<VulkanModel>(vulkanDevice, vertices);
	VulkanObject	triangle = VulkanObject::createVulkanObject();

	triangle.model = model;
	triangle.color = {0.1f, 0.8f, 0.1f};
	triangle.transform2d.translation.x = 0.0f;
	triangle.transform2d.scale = {1.0f, 1.0f};
	triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

	objects.emplace_back(std::move(triangle));
}

}