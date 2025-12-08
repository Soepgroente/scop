#pragma once

#include "VulkanObject.hpp"

namespace ve {

class VulkanTexture
{
	public:

	VulkanTexture() = delete;
	VulkanTexture(const std::string& filePath, VulkanDevice& device);
	~VulkanTexture() = default;

	VulkanTexture(const VulkanTexture& other);
	VulkanTexture&	operator=(const VulkanTexture& other);

	const ImageInfo&	getImageInfo() const noexcept { return imageInfo; }

	private:

	ImageInfo		imageInfo;
	VkDeviceSize	imageSize;
	VulkanDevice&	device_;
};

} // namespace ve