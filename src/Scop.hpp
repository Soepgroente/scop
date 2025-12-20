#pragma once

#include "Vectors.hpp"
#include "VulkanDevice.hpp"
#include "VulkanDescriptors.hpp"
#include "VulkanObject.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanRenderSystem.hpp"
#include "VulkanTexture.hpp"
#include "VulkanUtils.hpp"
#include "VulkanWindow.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <GLFW/glfw3.h>
#include <array>
#include <memory>
#include <vector>

namespace ve {

class Scop
{
	public:

	Scop() = delete;
	Scop(std::string objPath);
	~Scop();
	
	Scop(const Scop&) = delete;
	Scop& operator=(const Scop&) = delete;

	void	run();

	static constexpr int	DEFAULT_WIDTH = 1080;
	static constexpr int	DEFAULT_HEIGHT = 1080;

	private:

	void	loadObjects();

	VulkanWindow	vulkanWindow{DEFAULT_HEIGHT, DEFAULT_WIDTH, "Scop"};
	VulkanDevice	vulkanDevice{vulkanWindow};
	VulkanRenderer	vulkanRenderer{vulkanWindow, vulkanDevice};
	std::unique_ptr<VulkanDescriptorPool>	globalDescriptorPool{};

	std::string					objModelPath;
	std::vector<VulkanObject>	objects;
	std::vector<VulkanTexture>	textures;
	bool						rotateModel;
};

std::vector<ObjInfo>	parseOBJFile(const std::string& objFilePath);

vec3	generateRandomColor();
vec3	generateRandomGreyscale();
vec3	generateSoftGreyscale();
ImageInfo	loadImage(const std::string& imagePath);

}