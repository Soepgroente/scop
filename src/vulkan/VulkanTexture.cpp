#include "VulkanTexture.hpp"

namespace ve {

VulkanTexture::VulkanTexture(const std::string& filePath, VulkanDevice& device) : device_(device)
{
	imageInfo = loadImage(filePath);
	if (imageInfo.imageData == nullptr)
	{
		throw std::runtime_error("failed to load texture image!");
	}
	imageSize = static_cast<VkDeviceSize>(imageInfo.width) * imageInfo.height * 4;
}

VulkanTexture::VulkanTexture(const VulkanTexture& other) : device_(other.device_)
{
	imageInfo = other.imageInfo;
	imageSize = other.imageSize;
}

VulkanTexture&	VulkanTexture::operator=(const VulkanTexture& other)
{
	if (this != &other)
	{
		imageInfo = other.imageInfo;
		imageSize = other.imageSize;
	}
	return *this;
}

} // namespace ve