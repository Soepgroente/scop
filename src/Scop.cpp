#include "Scop.hpp"
#include "Camera.hpp"
#include "KeyboardInput.hpp"
#include "MouseInput.hpp"

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
	if (objects.size() > 1)
	{
		std::cerr << "More than one object is out of... Scop... for this project. Get it? Scop. 🥁" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

Scop::~Scop()
{
}

void	Scop::run()
{
	VulkanRenderSystem	renderSystem{vulkanDevice, vulkanRenderer.getSwapChainRenderPass()};
	Camera camera{};

	camera.setViewTarget(glm::vec3{-1.0f, -2.0f, 2.0f}, glm::vec3{0.0f, 0.0f, 2.5f});
	
	std::chrono::high_resolution_clock::time_point	currentTime, newTime;
	float	elapsedTime = 0.0f;
	float	aspectRatio = 1.0f;
	VkCommandBuffer	commandBuffer = nullptr;

	VulkanObject	viewerObject = VulkanObject::createVulkanObject();
	KeyboardInput	keyboardInput{};
	MouseInput		mouseInput{};

	currentTime = std::chrono::high_resolution_clock::now();
	rotateModel = true;
	int frameCount = 0;

	while (vulkanWindow.shouldClose() == false)
	{
		frameCount = (frameCount + 1) % 1000;
		glfwPollEvents();
		newTime = std::chrono::high_resolution_clock::now();
		elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;

		std::cout << "\rFrames per second: " << static_cast<int> (1.0f / elapsedTime) << " " << std::flush;
	
		keyboardInput.move(vulkanWindow.getGLFWwindow(), viewerObject, elapsedTime);
		mouseInput.move(vulkanWindow.getGLFWwindow(), viewerObject, elapsedTime);
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
	}
	vkDeviceWaitIdle(vulkanDevice.device());
}

static std::unique_ptr<VulkanModel> createCubeModel(VulkanDevice& device, glm::vec3 offset)
{
	VulkanModel::Builder	modelBuilder{};

	modelBuilder.vertices =
	{
		// left face (white)
		{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
		{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

		// right face (yellow)
		{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
		{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

		// top face (orange, remember y axis points down)
		{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

		// bottom face (red)
		{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
		{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

		// nose face (blue)
		{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

		// tail face (green)
		{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}}
	};

	for (VulkanModel::Vertex& vert : modelBuilder.vertices)
	{
		vert.pos += offset;
	}
	modelBuilder.indices = {0, 1, 2, 0, 3, 1, 4, 5, 6, 4, 7, 5, 8, 9, 10, 8, 11, 9,	12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
	return std::make_unique<VulkanModel>(device, modelBuilder);
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
