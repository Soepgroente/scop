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

	std::shared_ptr<VulkanModel>	model;
	glm::vec3			color;
	TransformComponent	transform{};
	
	const std::string&	getName() const noexcept { return name; }
	
	void	setName(const std::string& objName) { name = objName; }
	id_t	getID() const noexcept { return id; }
	
	private:

	VulkanObject(id_t objID);

	id_t		id;
	std::string	name;
};

} // namespace ve