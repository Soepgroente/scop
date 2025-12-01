#include "Scop.hpp"
#include <random>
#include <chrono>

namespace ve {

glm::vec3	generateRandomColor()
{
	static std::default_random_engine			engine
		(std::chrono::system_clock::now().time_since_epoch().count());
	static std::uniform_real_distribution<float>	distribution(0.0f, 1.0f);

	return glm::vec3
	(
		distribution(engine),
		distribution(engine),
		distribution(engine)
	);
}

} // namespace ve