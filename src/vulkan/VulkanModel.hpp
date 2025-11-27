#pragma once

#include "VulkanDevice.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <cassert>
#include <cstring>
#include <memory>
#include <vector>

namespace ve {

class VulkanModel
{
	public:

	struct Vertex
	{
		glm::vec3	pos;
		glm::vec3	color;
		// glm::vec3	normal;
		// glm::vec2	uv{};

		static std::vector<VkVertexInputBindingDescription>		getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription>	getAttributeDescriptions();
	};

	struct Material
	{
		std::string	name;
		glm::vec3	ambientColor;
		glm::vec3	diffuseColor;
		glm::vec3	specularColor;
		float		shininess;
		float		opacity;
		float		illuminationModel;
	};

	struct Builder
	{
		std::vector<Vertex>		vertices{};
		std::vector<uint32_t>	indices{};

		void	loadModel(const std::string& filepath);
	};

	VulkanModel() = delete;
	VulkanModel(VulkanDevice& vulkanDevice,	const VulkanModel::Builder& builder);
	~VulkanModel();

	VulkanModel(const VulkanModel&) = delete;
	VulkanModel& operator=(const VulkanModel&) = delete;

	static std::unique_ptr<VulkanModel>	createModelFromFile(
		VulkanDevice& device,
		const std::string& filepath
	);

	void	bind(VkCommandBuffer commandBuffer);
	void	draw(VkCommandBuffer commandBuffer);

	private:

	void	createVertexBuffers(const std::vector<Vertex>& vertices);
	void	createIndexBuffers(const std::vector<uint32_t>& indices);

	VulkanDevice&		vulkanDevice;
	VkBuffer			vertexBuffer;
	VkDeviceMemory		vertexBufferMemory;
	uint32_t			vertexCount;

	bool				hasIndexBuffer = false;
	VkBuffer			indexBuffer;
	VkDeviceMemory		indexBufferMemory;
	uint32_t			indexCount;
};

}