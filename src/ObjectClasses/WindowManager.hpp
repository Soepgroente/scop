#pragma once

#include "scop.hpp"

class WindowManager
{
	public:

	WindowManager();
	~WindowManager();
	WindowManager(const WindowManager& other) = delete;
	WindowManager& operator=(const WindowManager& other) = delete;

	void	initialize(int width, int height, const char* title);
	void	run();
	void	setScene(Scene& scene) { this->scene = &scene; }

	private:

	GLFWwindow*	window;

	int	width;
	int height;

	Scene*	scene;
};