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

struct GlobalUBO
{
	glm::mat4	projectionView{1.0f};
	glm::vec3	lightDirection = glm::normalize(glm::vec3{1.0f, -3.0f, -1.0f});
};

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
	std::vector<std::unique_ptr<VulkanBuffer>>	uboBuffers(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
	for (size_t i = 0; i < uboBuffers.size(); i++)
	{
		uboBuffers[i] = std::make_unique<VulkanBuffer>(
			vulkanDevice,
			sizeof(GlobalUBO),
			1,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		);
		uboBuffers[i]->map();
	}

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
	size_t			frameCount = 0;
	size_t			lastPressed = 0;

	currentTime = std::chrono::high_resolution_clock::now();
	rotateModel = false;
	viewerObject.transform.translation = {0.0f, 0.0f, -10.0f};
	while (vulkanWindow.shouldClose() == false)
	{
		glfwPollEvents();
		newTime = std::chrono::high_resolution_clock::now();
		elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;

		if (frameCount - lastPressed > 20 && glfwGetKey(vulkanWindow.getGLFWwindow(), GLFW_KEY_KP_ADD) == GLFW_PRESS)
		{
			rotateModel = !rotateModel;
			lastPressed = frameCount;
		}
		keyboardInput.move(vulkanWindow.getGLFWwindow(), viewerObject, elapsedTime);
		mouseInput.move(vulkanWindow.getGLFWwindow(), viewerObject, elapsedTime);
		camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
		aspectRatio = vulkanRenderer.getAspectRatio();
		camera.setPerspectiveProjection(glm::radians(50.0f), aspectRatio, 0.1f, 100.0f);
		
		commandBuffer = vulkanRenderer.beginFrame();
		if (commandBuffer != nullptr)
		{
			int frameIndex = vulkanRenderer.getCurrentFrameIndex();
			FrameInfo	info{frameIndex, elapsedTime, camera, commandBuffer};
			GlobalUBO	ubo{};

			ubo.projectionView = camera.getProjectionMatrix() * camera.getViewMatrix();
			uboBuffers[frameIndex]->writeToBuffer(&ubo);
			uboBuffers[frameIndex]->flush();

			vulkanRenderer.beginSwapChainRenderPass(commandBuffer);
			renderSystem.renderObjects(info, objects, rotateModel);
			newTime = std::chrono::high_resolution_clock::now();
			std::cout << "\rFrames per second: " << static_cast<int> (1.0f / elapsedTime) << ", Frame time: ";
			std::cout << std::chrono::duration<float, std::chrono::milliseconds::period>(newTime - currentTime).count() << "ms " << std::flush;		
			vulkanRenderer.endSwapChainRenderPass(commandBuffer);
			vulkanRenderer.endFrame();
		}
		frameCount++;
	}
	vkDeviceWaitIdle(vulkanDevice.device());
}

void	Scop::loadObjects()
{
	std::shared_ptr<VulkanModel>	model = VulkanModel::createModelFromFile(vulkanDevice, objModelPath);
	VulkanObject					object = VulkanObject::createVulkanObject();

	object.model = std::move(model);
	object.color = {1.0f, 0.4f, 0.2f};
	objects.push_back(std::move(object));
	textures.reserve(1);
	for (size_t i = 0; i < 1; i++)
	{
		VulkanTexture	texture("textures/derp" + std::to_string(i + 1) + ".jpeg", vulkanDevice);

		textures.emplace_back(std::move(texture));
	}
}

}	// namespace ve
