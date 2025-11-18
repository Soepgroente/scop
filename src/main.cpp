#include "scop.hpp"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <path_to_obj_file>" << std::endl;
		return 1;
	}
	Scene scene;
	ObjectParser::addObjectsFromFile(argv[1], scene);
	WindowManager windowManager;

	windowManager.setScene(scene);
	std::cout << "Loaded scene with " << scene.size() << " object(s)." << std::endl;
	windowManager.run();
	return 0;
}