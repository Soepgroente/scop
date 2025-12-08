#include "VulkanModel.hpp"
// #include "ObjectParser.hpp"
#include "VulkanObject.hpp"
#include "Scop.hpp"

#include <iostream>

namespace ve {

VulkanModel::VulkanModel(VulkanDevice& device, const VulkanModel::Builder& builder) : vulkanDevice{device}
{
	createVertexBuffers(builder.vertices);
	createIndexBuffers(builder.indices);
	vertexCount = static_cast<uint32_t>(builder.vertices.size());
	indexCount = static_cast<uint32_t>(builder.indices.size());
}

VulkanModel::~VulkanModel()
{
	vkFreeMemory(vulkanDevice.device(), vertexBufferMemory, nullptr);
	vkDestroyBuffer(vulkanDevice.device(), vertexBuffer, nullptr);

	if (hasIndexBuffer == true)
	{
		vkDestroyBuffer(vulkanDevice.device(), indexBuffer, nullptr);
		vkFreeMemory(vulkanDevice.device(), indexBufferMemory, nullptr);
	}
}

void	VulkanModel::createVertexBuffers(const std::vector<Vertex>& vertices)
{
	vertexCount = static_cast<uint32_t>(vertices.size());

	assert(vertexCount >= 3 && "Vertex count must be at least 3");

	VkDeviceSize	bufferSize = sizeof(vertices[0]) * vertexCount;
	VkBuffer		stagingBuffer;
	VkDeviceMemory	stagingBufferMemory;

	vulkanDevice.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory
	);
	void*	data;

	vkMapMemory(vulkanDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	std::memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(vulkanDevice.device(), stagingBufferMemory);

	vulkanDevice.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBuffer,
		vertexBufferMemory
	);
	vulkanDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
	vkDestroyBuffer(vulkanDevice.device(), stagingBuffer, nullptr);
	vkFreeMemory(vulkanDevice.device(), stagingBufferMemory, nullptr);
}

void	VulkanModel::createIndexBuffers(const std::vector<uint32_t>& indices)
{
	indexCount = static_cast<uint32_t>(indices.size());
	hasIndexBuffer = indexCount > 0;

	if (hasIndexBuffer == false)
	{
		return;
	}
	VkDeviceSize	bufferSize = sizeof(indices[0]) * indexCount;
	VkBuffer		stagingBuffer;
	VkDeviceMemory	stagingBufferMemory;

	vulkanDevice.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory
	);
	void*	data;

	vkMapMemory(vulkanDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	std::memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(vulkanDevice.device(), stagingBufferMemory);

	vulkanDevice.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		indexBuffer,
		indexBufferMemory
	);
	vulkanDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);
	vkDestroyBuffer(vulkanDevice.device(), stagingBuffer, nullptr);
	vkFreeMemory(vulkanDevice.device(), stagingBufferMemory, nullptr);
}

void	VulkanModel::bind(VkCommandBuffer commandBuffer)
{
	VkBuffer		buffers[] = {vertexBuffer};
	VkDeviceSize	offsets[] = {0};

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	if (hasIndexBuffer == true)
	{
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
}

void	VulkanModel::draw(VkCommandBuffer commandBuffer)
{
	if (hasIndexBuffer == true)
	{
		vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
	}
	else
	{
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}
}

std::vector<VkVertexInputBindingDescription>	VulkanModel::Vertex::getBindingDescriptions()
{
	std::vector<VkVertexInputBindingDescription>	bindingDescriptions(1);

	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription>	VulkanModel::Vertex::getAttributeDescriptions()
{
	std::vector<VkVertexInputAttributeDescription>	attributeDescriptions(2);

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, pos);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);
	return attributeDescriptions;
}

glm::vec3	VulkanModel::calculateBoundingCenter(const std::vector<Vertex>& vertices) noexcept
{
	float	xMin, xMax,
			yMin, yMax,
			zMin, zMax;

	xMin = std::min_element(vertices.begin(), vertices.end(),
		[](const Vertex& a, const Vertex& b) { return a.pos.x < b.pos.x; })->pos.x;
	xMax = std::max_element(vertices.begin(), vertices.end(),
		[](const Vertex& a, const Vertex& b) { return a.pos.x < b.pos.x; })->pos.x;
	yMin = std::min_element(vertices.begin(), vertices.end(),
		[](const Vertex& a, const Vertex& b) { return a.pos.y < b.pos.y; })->pos.y;
	yMax = std::max_element(vertices.begin(), vertices.end(),
		[](const Vertex& a, const Vertex& b) { return a.pos.y < b.pos.y; })->pos.y;
	zMin = std::min_element(vertices.begin(), vertices.end(),
		[](const Vertex& a, const Vertex& b) { return a.pos.z < b.pos.z; })->pos.z;
	zMax = std::max_element(vertices.begin(), vertices.end(),
		[](const Vertex& a, const Vertex& b) { return a.pos.z < b.pos.z; })->pos.z;

	return glm::vec3 {
		(xMin + xMax) / 2.0f,
		(yMin + yMax) / 2.0f,
		(zMin + zMax) / 2.0f
	};
}

glm::vec3	VulkanModel::calculateVertexCenter(const std::vector<Vertex>& vertices) noexcept
{
	glm::vec3	center{};

	for (const Vertex& vertex : vertices)
	{
		center += vertex.pos;
	}
	center /= static_cast<float>(vertices.size());
	return center;
}

std::unique_ptr<VulkanModel>	VulkanModel::createModelFromFile(VulkanDevice& device, const std::string& filepath)
{
	Builder	builder{};
	builder.loadModel(filepath);

	std::unique_ptr<VulkanModel>	model = std::make_unique<VulkanModel>(device, builder);

	model->vertexCenter = model->calculateVertexCenter(builder.vertices);
	model->boundingCenter = model->calculateBoundingCenter(builder.vertices);
	return model;
}

void	VulkanModel::Builder::loadModel(const std::string &filepath)
{
	std::vector<ObjInfo>		objs = parseOBJFile(filepath);

	vertices.clear();
	indices.clear();

	std::unordered_map<Vertex, uint32_t>	uniqueVertices{};

	for (const ObjInfo& obj : objs)
	{
		for (const ObjComponent& component : obj.components)
		{
			for (size_t i = 0; i < component.faceIndices.size(); i++)
			{
				const std::vector<uint32_t>& face = component.faceIndices[i];
				const std::vector<uint32_t>& tex = component.textureIndices[i];
				const std::vector<uint32_t>& norm = component.normalIndices[i];

				if (face.size() < 3)
				{
					continue;
				}

				/*	If more than 3 faces, create triangles	*/

				for (size_t j = 2; j < face.size(); j++)
				{
					for (size_t ti : {0UL, j - 1, j})
					{
						Vertex	vertex{};

						vertex.pos = obj.vertices[face[ti]];
						if (tex.size() > ti)
						{
							vertex.textureUv = obj.textureCoords[tex[ti]];
						}
						if (norm.size() > ti)
						{
							vertex.normal = obj.normals[norm[ti]];
						}
						vertex.color = generateRandomColor();
						if (uniqueVertices.count(vertex) == 0)
						{
							uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
							vertices.push_back(vertex);
						}
						indices.push_back(uniqueVertices[vertex]);
					}
				}
			}
		}
	}

}

}	// namespace ve