#pragma once

#include "VulkanWindow.hpp"
#include "VulkanPipeline.hpp"

namespace ve {

class Scop
{
	public:

	Scop() = default;
	Scop(const Scop&) = delete;
	Scop& operator=(const Scop&) = delete;
	~Scop() = default;

	void	run();

	static constexpr int	DEFAULT_WIDTH = 1440;
	static constexpr int	DEFAULT_HEIGHT = 1440;

	private:
	
	VulkanWindow	vulkanWindow{DEFAULT_HEIGHT, DEFAULT_WIDTH, "Scop"};
	VulkanPipeline	vulkanPipeline{"src/shaders/shadyBusiness.vert.spv", "src/shaders/shadyBusiness.frag.spv"};
};

}