#pragma once

#include "Scop.hpp"

namespace ve {

class VulkanTexture
{
	public:

	VulkanTexture() = delete;
	VulkanTexture(const std::string& filePath);
	~VulkanTexture() = default;

	VulkanTexture(const VulkanTexture& other) = delete;
	VulkanTexture&	operator=(const VulkanTexture& other) = delete;

	const ImageInfo&	getImageInfo() const noexcept { return imageInfo; }

	private:

	ImageInfo		imageInfo;
	VkDeviceSize	imageSize;
	VulkanDevice*	device;
};

} // namespace ve