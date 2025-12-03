#pragma once

#include "VulkanDevice.hpp"
#include "VulkanObject.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanRenderSystem.hpp"
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

struct ImageInfo
{
	const unsigned char*	imageData;
	int		width;
	int		height;
	int		channels;
};

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

	std::string					objModelPath;
	std::vector<VulkanObject>	objects;
	std::vector<ImageInfo>		textures;
	bool						rotateModel;

};

std::vector<ObjInfo>	parseOBJFile(const std::string& objFilePath);

glm::vec3	generateRandomColor();
glm::vec3	generateRandomGreyscale();
ImageInfo	loadImage(const std::string& imagePath);

}