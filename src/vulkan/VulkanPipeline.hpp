#pragma once

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace ve {

class VulkanPipeline
{
	public:

	VulkanPipeline() = delete;
	VulkanPipeline(const std::string& vertPath, const std::string& fragPath);
	VulkanPipeline(const VulkanPipeline&) = delete;
	VulkanPipeline& operator=(const VulkanPipeline&) = delete;
	~VulkanPipeline() = default;

	private:

	static std::vector<char>	readFile(const std::string& filePath);

	void	createGraphicsPipeline(const std::string& vertPath, const std::string& fragPath);
};

}