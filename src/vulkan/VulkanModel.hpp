#pragma once

#include "VulkanBuffer.hpp"
#include "VulkanDevice.hpp"
#include "VulkanUtils.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <vulkan/vulkan.h>

#include <cassert>
#include <cstring>
#include <memory>
#include <vector>
#include <unordered_map>

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

	glm::vec3	getVertexCenter() const noexcept { return vertexCenter; }
	glm::vec3	getBoundingCenter() const noexcept { return boundingCenter; }
	static std::unique_ptr<VulkanModel>	createModelFromFile(VulkanDevice& device, const std::string& filepath);

	private:
	
	std::string			name;
	bool				hasIndexBuffer = false;

	VulkanDevice&		vulkanDevice;
	uint32_t			vertexCount;

	std::unique_ptr<VulkanBuffer>	vertexBuffer;
	std::unique_ptr<VulkanBuffer>	indexBuffer;

	uint32_t			indexCount;

	glm::vec3			vertexCenter;
	glm::vec3			boundingCenter;

	void	createVertexBuffers(const std::vector<Vertex>& vertices);
	void	createIndexBuffers(const std::vector<uint32_t>& indices);

	static glm::vec3	calculateVertexCenter(const std::vector<Vertex>& vertices) noexcept;
	static glm::vec3	calculateBoundingCenter(const std::vector<Vertex>& vertices) noexcept;
};

}

namespace std {

template<>
struct hash<ve::VulkanModel::Vertex>
{
	size_t operator()(ve::VulkanModel::Vertex const& vertex) const
	{
		size_t seed = 0;

		ve::hashCombine(seed, vertex.pos, vertex.color, vertex.normal, vertex.textureUv);
		return seed;
	}
};

}	// namespace std