#pragma once

#include "VulkanModel.hpp"

#include <memory>

namespace ve {

struct Transform2dComponent
{
	glm::vec2	translation{};
	glm::vec2	scale{1.0f, 1.0f};
	float		rotation;

	glm::mat2	mat2()
	{
		const float s = glm::sin(rotation);
		const float c = glm::cos(rotation);

		glm::mat2 rotationMat{{c, s}, {-s, c}};
		glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};

		return rotationMat * scaleMat;
	}
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
	glm::vec3				color;
	Transform2dComponent	transform2d;

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