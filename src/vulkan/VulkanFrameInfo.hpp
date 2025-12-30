#pragma once

#include "Camera.hpp"
#include "VulkanObject.hpp"

namespace ve {

struct FrameInfo
{
	int					frameIndex;
	float				frameTime;
	Camera&				camera;
	VkCommandBuffer		commandBuffer;
	VkDescriptorSet		globalDescriptorSet;
	VulkanObject::Map&	gameObjects;
};

}	// namespace ve