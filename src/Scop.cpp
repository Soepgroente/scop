#include "Scop.hpp"
#include "Camera.hpp"
#include "KeyboardInput.hpp"

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

Scop::~Scop()
{
}

void	Scop::run()
{
	VulkanRenderSystem	renderSystem{vulkanDevice, vulkanRenderer.getSwapChainRenderPass()};
	Camera camera{};

	// camera.setViewDirection(glm::vec3{0.0f}, glm::vec3{0.5f, 0.0f, 1.0f});
	camera.setViewTarget(glm::vec3{-1.0f, -2.0f, 2.0f}, glm::vec3{0.0f, 0.0f, 2.5f});
	
	std::chrono::high_resolution_clock::time_point	currentTime, newTime;
	float	elapsedTime = 0.0f;
	float	aspectRatio = 1.0f;
	VkCommandBuffer	commandBuffer = nullptr;

	VulkanObject viewerObject = VulkanObject::createVulkanObject();
	KeyboardInput	keyboardInput{};

	currentTime = std::chrono::high_resolution_clock::now();
	while (vulkanWindow.shouldClose() == false)
	{
		glfwPollEvents();
		newTime = std::chrono::high_resolution_clock::now();
		elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;
		keyboardInput.move(vulkanWindow.getGLFWwindow(), viewerObject, elapsedTime);
		camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
		aspectRatio = vulkanRenderer.getAspectRatio();
		camera.setPerspectiveProjection(glm::radians(50.0f), aspectRatio, 0.1f, 100.0f);

		commandBuffer = vulkanRenderer.beginFrame();
		if (commandBuffer != nullptr)
		{
			vulkanRenderer.beginSwapChainRenderPass(commandBuffer);
			renderSystem.renderObjects(commandBuffer, objects, camera);
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

static std::unique_ptr<VulkanModel> createCubeModel(VulkanDevice& device, glm::vec3 offset)
{
	std::vector<VulkanModel::Vertex> vertices {

	// left face (white)
	{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
	{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
	{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
	{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
	{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
	{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

	// right face (yellow)
	{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
	{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
	{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
	{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
	{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
	{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

	// top face (orange, remember y axis points down)
	{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
	{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
	{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
	{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
	{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
	{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

	// bottom face (red)
	{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
	{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
	{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
	{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
	{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
	{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

	// nose face (blue)
	{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
	{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
	{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
	{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
	{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
	{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

	// tail face (green)
	{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
	{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
	{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
	{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
	{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
	{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}}
	
	};
	for (auto& v : vertices)
	{
		v.pos += offset;
	}
	return std::make_unique<VulkanModel>(device, vertices);
}

void	Scop::loadObjects()
{
	VulkanObject	cube = VulkanObject::createVulkanObject();

	cube.model = createCubeModel(vulkanDevice, glm::vec3{0.0f, 0.0f, 0.0f});
	cube.transform.translation = glm::vec3{0.0f, 0.0f, 2.5f};
	cube.transform.scale = glm::vec3{0.5f, 0.5f, 0.5f};
	objects.push_back(std::move(cube));
}

} // namespace ve
