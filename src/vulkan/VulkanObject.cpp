#include "VulkanObject.hpp"

namespace ve {

id_t	VulkanObject::currentID = 0;

VulkanObject::VulkanObject(id_t objID) : id(objID)
{
}

VulkanObject::~VulkanObject()
{
}

// void	VulkanObject::setMaterial(const Material& material)
// {
// 	if (mat.exists == true)
// 	{
// 		throw std::runtime_error("Material already set for VulkanObject " + name);
// 	}
// 	mat = material;
// }

} // namespace ve