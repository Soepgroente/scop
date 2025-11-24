#pragma once

#include "VulkanModel.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace ve {

struct TransformComponent
{
	glm::vec3	translation{};
	glm::vec3	scale{1.0f, 1.0f, 1.0f};
	glm::vec3	rotation{};

	glm::mat4	mat4();
};

class VulkanObject
{
	public:

	using id_t = uint32_t;

	VulkanObject() = delete;
	~VulkanObject();
	VulkanObject(const VulkanObject& other) = delete;
	VulkanObject& operator=(const VulkanObject& other) = delete;
	VulkanObject(VulkanObject&& other) = default;
	VulkanObject& operator=(VulkanObject&& other) = default;

	static VulkanObject	createVulkanObject() { return VulkanObject(++currentID); }
	static id_t		currentID;

	id_t	getID() const { return id; }

	std::shared_ptr<VulkanModel>	model;
	glm::vec3			color;
	TransformComponent	transform{};

	// struct Material
	// {
	// 	bool		exists = false;
	// 	std::string	name;
	// 	glm::vec3	ambientClr;
	// 	glm::vec3	diffuseClr;
	// 	glm::vec3	specularClr;
	// 	float		specularExp;
	// 	float		transparency;
	// 	int			illumModel;
	// };

	// struct Mesh
	// {
	// 	std::vector<ve::VulkanModel::Vertex>	vertices;
	// 	std::vector<std::vector<int>>	faces;
	// 	std::vector<glm::vec3>	normals;
	// 	std::vector<glm::vec3>	textureCoords;
	// };

	// void	setName(const std::string& objName) { name = objName; }
	// void	addFace(const std::vector<int>& face) { mesh.faces.push_back(face); }
	// void	addSurfaceNormal(const glm::vec3& normal) { mesh.normals.push_back(normal); }
	// void	addTextureCoord(const glm::vec3& texCoord) { mesh.textureCoords.push_back(texCoord); }
	// void	addVertex(const ve::VulkanModel::Vertex& vertex) { mesh.vertices.push_back(vertex); }

	// void	setMaterial(const Material& material);

	// const std::string&	getName() const { return name; }
	// std::vector<ve::VulkanModel::Vertex>	renderData() const { return mesh.vertices; }
	
	private:

	VulkanObject(id_t objID);

	id_t		id;
	// std::string	name;
	// Mesh		mesh;
	// Material	mat;
};

} // namespace ve