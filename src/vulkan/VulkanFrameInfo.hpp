#pragma once

#include "Camera.hpp"

#include <vulkan/vulkan.h>

namespace ve {

struct FrameInfo
{
	int				frameIndex;
	float			frameTime;
	Camera&			camera;
	VkCommandBuffer	commandBuffer;
};

}	// namespace ve