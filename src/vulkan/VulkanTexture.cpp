#include "VulkanTexture.hpp"

namespace ve {

VulkanTexture::VulkanTexture(const std::string& filePath)
{
	imageInfo = loadImage(filePath);
	if (imageInfo.imageData == nullptr)
	{
		throw std::runtime_error("failed to load texture image!");
	}
	imageSize = static_cast<VkDeviceSize>(imageInfo.width) * imageInfo.height * 4;
}

} // namespace ve