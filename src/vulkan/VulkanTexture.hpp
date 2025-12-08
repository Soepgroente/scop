#pragma once

#include "VulkanObject.hpp"

namespace ve {

class VulkanTexture
{
	public:

	VulkanTexture() = delete;
	VulkanTexture(const std::string& filePath, VulkanDevice& device);
	~VulkanTexture();

	VulkanTexture(const VulkanTexture& other);
	VulkanTexture&	operator=(const VulkanTexture& other);

	void	createTextureImage();
	void	createTextureImageView();
	void	createTextureSampler();

	const ImageInfo&	getImageInfo() const noexcept { return imageInfo; }

	private:

	ImageInfo		imageInfo;
	VkDeviceSize	imageSize;
	VkImage			textureImage;
	VkBuffer		stagingBuffer;
	VkDeviceMemory	stagingBufferMemory;
	
	VkImageView		textureImageView;
	VkSampler		textureSampler;

	VkImageCreateInfo	info{};

	VulkanDevice&	device;
};

} // namespace ve