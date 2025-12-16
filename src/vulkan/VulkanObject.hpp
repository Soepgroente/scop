#pragma once

#include "VulkanModel.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <map>

namespace ve {

struct ImageInfo
{
	const unsigned char*	imageData;
	int		width;
	int		height;
	int		channels;
};

struct Material
{
	std::string	name;
	glm::vec3	ambientClr;
	glm::vec3	diffuseClr;
	glm::vec3	specularClr;
	float		shininess;
	float		opacity;
	float		illuminationModel;
	float		refractionIndex;
	bool		smoothShading;
};

struct ObjComponent
{
	std::vector<std::vector<uint32_t>>	faceIndices;
	std::vector<std::vector<uint32_t>>	textureIndices;
	std::vector<std::vector<uint32_t>>	normalIndices;
	std::string				matName;
};

struct ObjInfo
{
	std::string					name;
	std::string					mtlFile;
	std::vector<glm::vec3>		vertices;
	std::vector<glm::vec2>		textureCoords;
	std::vector<glm::vec3>		normals;
	std::vector<ObjComponent>	components;
	std::map<std::string, Material>		materials;
};

struct TransformComponent
{
	glm::vec3	translation{};
	glm::vec3	scale{1.0f, 1.0f, 1.0f};
	glm::vec3	rotation{};

	glm::mat4	mat4() const noexcept;
	glm::mat4	mat4(const glm::vec3& rotationCenter) const noexcept;
	glm::mat3	normalMatrix() const noexcept;
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

	std::shared_ptr<VulkanModel>	model;
	glm::vec3						color;
	TransformComponent				transform{};
	
	id_t	getID() const noexcept { return id; }
	
	private:

	VulkanObject(id_t objID);

	id_t		id;
};

std::ostream&	operator<<(std::ostream& os, const ObjInfo& obj);
ImageInfo		loadImage(const std::string& imagePath);

} // namespace ve