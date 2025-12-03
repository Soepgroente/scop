#include "Scop.hpp"
#include <random>
#include <chrono>

namespace ve {

static float	randomFloat()
{
	static std::default_random_engine			engine
		(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<float>		distribution(0.0f, 1.0f);

	return distribution(engine);
}

glm::vec3	generateRandomColor()
{
	return glm::vec3(randomFloat(), randomFloat(), randomFloat());
}

glm::vec3	generateRandomGreyscale()
{
	float	grey = randomFloat();

	return glm::vec3(grey, grey, grey);
}

} // namespace ve