#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "VulkanObject.hpp"
#include "VulkanModel.hpp"

namespace ve {

class ObjectParser
{
	public:

	static std::vector<VulkanObject>	parseOBJFile(const std::string& objFilePath);

	ObjectParser() = delete;
	~ObjectParser() = delete;
};

} // namespace ve