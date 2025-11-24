#pragma once

#include "VulkanDevice.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <cassert>
#include <cstring>
#include <vector>

namespace ve {

class VulkanModel
{
	public:

	struct Vertex
	{
		glm::vec3	pos;
		glm::vec3	color;

		static std::vector<VkVertexInputBindingDescription>		getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription>	getAttributeDescriptions();
	};

	VulkanModel() = delete;
	VulkanModel(VulkanDevice& vulkanDevice,	const std::vector<Vertex>& vertices);
	~VulkanModel();

	VulkanModel(const VulkanModel&) = delete;
	VulkanModel& operator=(const VulkanModel&) = delete;

	void	bind(VkCommandBuffer commandBuffer);
	void	draw(VkCommandBuffer commandBuffer);

	private:

	void	createVertexBuffers(const std::vector<Vertex>& vertices);

	VulkanDevice&		vulkanDevice;
	VkBuffer			vertexBuffer;
	VkDeviceMemory		vertexBufferMemory;
	uint32_t			vertexCount;
};

}