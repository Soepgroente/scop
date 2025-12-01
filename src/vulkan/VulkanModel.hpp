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
		glm::vec3	normal;
		glm::vec2	textureUv{};

		static std::vector<VkVertexInputBindingDescription>		getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription>	getAttributeDescriptions();

		bool operator==(const Vertex& other) const noexcept
		{
			return	pos == other.pos &&
					color == other.color &&
					normal == other.normal &&
					textureUv == other.textureUv;
		}
	};

	struct Builder
	{
		std::vector<Vertex>		vertices{};
		std::vector<uint32_t>	indices{};

		void	loadModel(const std::string &filepath);
	};

	VulkanModel() = delete;
	VulkanModel(VulkanDevice& vulkanDevice,	const VulkanModel::Builder& builder);
	~VulkanModel();

	VulkanModel(const VulkanModel&) = delete;
	VulkanModel& operator=(const VulkanModel&) = delete;

	void	bind(VkCommandBuffer commandBuffer);
	void	draw(VkCommandBuffer commandBuffer);

	void	setName(const std::string& name) { this->name = name; }

	static std::unique_ptr<VulkanModel>	createModelFromFile(VulkanDevice& device, const std::string& filepath);

	private:
	
	std::string			name;
	bool				hasIndexBuffer = false;

	VulkanDevice&		vulkanDevice;
	VkBuffer			vertexBuffer;
	VkDeviceMemory		vertexBufferMemory;
	uint32_t			vertexCount;
	
	VkBuffer			indexBuffer;
	VkDeviceMemory		indexBufferMemory;
	uint32_t			indexCount;
	
	void	createVertexBuffers(const std::vector<Vertex>& vertices);
	void	createIndexBuffers(const std::vector<uint32_t>& indices);
};

}