#include "WindowManager.hpp"

typedef void (*PFNGLCLEARPROC)(GLbitfield mask);
typedef void (*PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (*PFNGLENABLEPROC)(GLenum cap);
typedef void (*PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);

PFNGLCLEARPROC glClear = nullptr;
PFNGLCLEARCOLORPROC glClearColor = nullptr;
PFNGLENABLEPROC glEnable = nullptr;
PFNGLVIEWPORTPROC glViewport = nullptr;

WindowManager::WindowManager() : width(1080), height(1080)
{
	window = nullptr;
	scene = nullptr;
	initialize(width, height, "scop");
}

WindowManager::~WindowManager()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool loadOpenGLFunctions()
{
    glClear = (PFNGLCLEARPROC)glfwGetProcAddress("glClear");
    glClearColor = (PFNGLCLEARCOLORPROC)glfwGetProcAddress("glClearColor");
    glEnable = (PFNGLENABLEPROC)glfwGetProcAddress("glEnable");
    glViewport = (PFNGLVIEWPORTPROC)glfwGetProcAddress("glViewport");
    
    return (glClear && glClearColor && glEnable && glViewport);
}

void WindowManager::initialize(int width, int height, const char* title)
{
    glfwSetErrorCallback([](int error_code, const char* description)
	{
		std::cerr << "GLFW Error (" << error_code << "): " << description << std::endl;
	});
	if (glfwInit() == false)
	{
		throw std::bad_alloc();
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);
    glfwMakeContextCurrent(window);

    if (loadOpenGLFunctions() == false)
    {
        std::cerr << "Failed to load OpenGL functions" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	});
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

void	WindowManager::run()
{
	while (glfwWindowShouldClose(this->window) == false)
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// render code goes here
		
		glfwSwapBuffers(this->window);
	}
}