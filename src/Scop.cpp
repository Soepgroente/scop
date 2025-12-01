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

	camera.setViewTarget(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f});
	
	std::chrono::high_resolution_clock::time_point	currentTime, newTime;
	float	elapsedTime = 0.0f;
	float	aspectRatio = 1.0f;
	VkCommandBuffer	commandBuffer = nullptr;

	VulkanObject	viewerObject = VulkanObject::createVulkanObject();
	KeyboardInput	keyboardInput{};
	MouseInput		mouseInput{};

	currentTime = std::chrono::high_resolution_clock::now();
	rotateModel = false;
	int frameCount = 0;

	viewerObject.transform.translation = {0.0f, 0.0f, -10.0f};
	while (vulkanWindow.shouldClose() == false)
	{
		frameCount = (frameCount + 1) % 1000;
		glfwPollEvents();
		newTime = std::chrono::high_resolution_clock::now();
		elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;

		if (glfwGetKey(vulkanWindow.getGLFWwindow(), GLFW_KEY_KP_ADD) == GLFW_PRESS)
		{
			rotateModel = !rotateModel;
		}
		keyboardInput.move(vulkanWindow.getGLFWwindow(), viewerObject, elapsedTime);
		mouseInput.move(vulkanWindow.getGLFWwindow(), viewerObject, elapsedTime);
		camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
		aspectRatio = vulkanRenderer.getAspectRatio();
		camera.setPerspectiveProjection(glm::radians(50.0f), aspectRatio, 0.1f, 100.0f);
		
		commandBuffer = vulkanRenderer.beginFrame();
		if (commandBuffer != nullptr)
		{
			vulkanRenderer.beginSwapChainRenderPass(commandBuffer);
			renderSystem.renderObjects(commandBuffer, objects, camera, rotateModel);
			newTime = std::chrono::high_resolution_clock::now();
			std::cout << "\rFrames per second: " << static_cast<int> (1.0f / elapsedTime) << ", Frame time: ";
			std::cout << std::chrono::duration<float, std::chrono::milliseconds::period>(newTime - currentTime).count() << "ms " << std::flush;		
			vulkanRenderer.endSwapChainRenderPass(commandBuffer);
			vulkanRenderer.endFrame();
		}
	}
	vkDeviceWaitIdle(vulkanDevice.device());
}

void	Scop::loadObjects()
{
	std::shared_ptr<VulkanModel>	model = VulkanModel::createModelFromFile(vulkanDevice, objModelPath);
	VulkanObject					object = VulkanObject::createVulkanObject();

	object.model = model;
	object.color = {1.0f, 0.4f, 0.2f};
	objects.push_back(std::move(object));
}

} // namespace ve
