#include "Scop.hpp"

namespace ve {


void	Scop::run()
{
	while (vulkanWindow.shouldClose() == false)
	{
		glfwPollEvents();

	}
}

}