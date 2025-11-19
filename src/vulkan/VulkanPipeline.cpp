#include "VulkanPipeline.hpp"

namespace ve {

VulkanPipeline::VulkanPipeline(const std::string& vertPath, const std::string& fragPath)
{
	createGraphicsPipeline(vertPath, fragPath);
}

std::vector<char>	VulkanPipeline::readFile(const std::string& filePath)
{
	std::ifstream	file(filePath, std::ios::ate | std::ios::binary);

	if (file.is_open() == false)
	{
		throw std::runtime_error("failed to open file: " + filePath);
	}
	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char>	buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

void	VulkanPipeline::createGraphicsPipeline(const std::string& vertPath, const std::string& fragPath)
{
	std::vector<char>	vertexShaderCode = readFile(vertPath);
	std::vector<char>	fragmentShaderCode = readFile(fragPath);


}

}