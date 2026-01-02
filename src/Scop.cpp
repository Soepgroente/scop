#include "Scop.hpp"
#include "Camera.hpp"
#include "KeyboardInput.hpp"
#include "MouseInput.hpp"

#include <chrono>
#include <random>

namespace ve {

struct GlobalUBO
{
	mat4				projectionView{1.0f};
	vec4				ambientLightColor{1.0f, 1.0f, 1.0f, 0.1f};
	vec3				lightPosition{0.0f, -4.0f, -3.0f};
	alignas(16)	vec4	lightColor{1.0f};
};

Scop::Scop(std::string objPath) : objModelPath(objPath)
{
	globalDescriptorPool = VulkanDescriptorPool::Builder(vulkanDevice)
		.setMaxSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
		.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VulkanSwapChain:: MAX_FRAMES_IN_FLIGHT)
		.build();
	loadObjects();
	if (objects.size() > 1)
	{
		std::cerr << "More than one object is out of... Scop... for this project. Get it? Scop. 🥁" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	loadDefaultObjects();
}

Scop::~Scop()
{
	globalDescriptorPool.reset();
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

	std::unique_ptr<VulkanDescriptorSetLayout> globalSetLayout = VulkanDescriptorSetLayout::Builder(vulkanDevice)
		.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
		.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
		.build();
	std::vector<VkDescriptorSet>	globalDescriptorSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
	VulkanTexture texture{"textures/defaultTex.jpg", vulkanDevice};

	for (size_t i = 0; i < globalDescriptorSets.size(); i++)
	{
		VkDescriptorBufferInfo bufferInfo = uboBuffers[i]->descriptorInfo();
		VkDescriptorImageInfo imageInfo{};

		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = texture.getImageView();
		imageInfo.sampler = texture. getSampler();

		VulkanDescriptorWriter(*globalSetLayout, *globalDescriptorPool)
			.writeBuffer(0, &bufferInfo)
			.writeImage(1, &imageInfo)
			.build(globalDescriptorSets[i]);
	}

	VulkanRenderSystem	renderSystem{vulkanDevice, vulkanRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
	Camera camera{};

	camera.setViewTarget(vec3{0.0f}, vec3{0.0f});
	
	float	elapsedTime = 0.0f;
	float	aspectRatio = 1.0f;
	std::chrono::high_resolution_clock::time_point	currentTime, newTime;
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
		camera.setPerspectiveProjection(radians(50.0f), aspectRatio, 0.1f, 1000.0f);
		
		commandBuffer = vulkanRenderer.beginFrame();
		if (commandBuffer != nullptr)
		{
			int frameIndex = vulkanRenderer.getCurrentFrameIndex();
			GlobalUBO	ubo{};
			FrameInfo	info{
				frameIndex,
				elapsedTime,
				camera,
				commandBuffer,
				globalDescriptorSets[frameIndex],
				objects
			};

			ubo.projectionView = camera.getProjectionMatrix() * camera.getViewMatrix();
			uboBuffers[frameIndex]->writeToBuffer(&ubo);
			uboBuffers[frameIndex]->flush();

			vulkanRenderer.beginSwapChainRenderPass(commandBuffer);
			renderSystem.renderObjects(info, rotateModel);
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

void	Scop::loadDefaultObjects()
{
	std::shared_ptr<VulkanModel>	planeModel = VulkanModel::createModelFromFile(vulkanDevice, "models/quad.obj");
	VulkanObject					floor = VulkanObject::createVulkanObject();

	floor.model = std::move(planeModel);
	floor.transform.translation.y = objects.at(1).model->getBoundingBox().min.y - objects.at(1).model->getBoundingCenter().y;
	floor.transform.scale = {10.0f, 1.0f, 10.0f};
	floor.color = {0.3f, 0.3f, 0.3f};
	objects.emplace(floor.getID(), std::move(floor));

	textures.emplace_back("textures/defaultTex.jpg", vulkanDevice);
}

void	Scop::loadObjects()
{
	std::shared_ptr<VulkanModel>	model = VulkanModel::createModelFromFile(vulkanDevice, objModelPath);
	VulkanObject					object = VulkanObject::createVulkanObject();

	object.model = std::move(model);
	object.color = {1.0f, 0.4f, 0.2f};
	object.transform.translation = object.model->getBoundingCenter().inverted();
	objects.emplace(object.getID(), std::move(object));
	textures.reserve(5);
	for (size_t i = 0; i < 5; i++)
	{
		VulkanTexture	texture("textures/derp" + std::to_string(i + 1) + ".jpeg", vulkanDevice);

		textures.emplace_back(std::move(texture));
	}
}

}	// namespace ve
