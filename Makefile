NAME			:=	scop
CC				:=	c++
BASE_CPPFLAGS	:=	-Wall -Wextra -Werror -std=c++20
RELEASE_FLAGS	:=	-DNDEBUG -flto -O3 -march=native
DEBUG_FLAGS		:=	-g -fsanitize=address

INCLUDES:=	-I./src -I./libs/glm \
			-I/opt/homebrew/include \
			-I./src/VulkanObjectClasses \
			-I./src/VectorClasses \
			-I./src/vulkan \
			-I./src \

SRCS	:=	main.cpp \
			Scop.cpp \
			vulkan/VulkanDevice.cpp \
			vulkan/VulkanModel.cpp \
			vulkan/VulkanObject.cpp \
			vulkan/VulkanPipeline.cpp \
			vulkan/VulkanRenderer.cpp \
			vulkan/VulkanRenderSystem.cpp \
			vulkan/VulkanSwapChain.cpp \
			vulkan/VulkanWindow.cpp \


SRCDIR	:=	src
OBJDIR	:=	$(SRCDIR)/obj
OBJS	:=	$(addprefix $(OBJDIR)/,$(notdir $(SRCS:%.cpp=%.o)))

SHADERS_SRC	:=	src/shaders/shadyBusiness.vert \
				src/shaders/shadyBusiness.frag \

GLSLC				:= $(shell which glslc)
SHADERS_COMPILED	:= $(SHADERS_SRC:%=%.spv)

LIBS		:=	-L/opt/homebrew/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit
RPATH_DIR	:=	/usr/local/lib
LDFLAGS		:=	-lvulkan -I/opt/homebrew/include -lm -Wl -rpath $(RPATH_DIR)

ifeq ($(UNAME_S), Linux)
	ECHO_MESSAGE = "Linux"
	LIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl `pkg-config --static --libs glfw3`
	BASE_CPPFLAGS += `pkg-config --cflags glfw3`
endif

CPPFLAGS = $(BASE_CPPFLAGS) $(RELEASE_FLAGS)

all: $(SHADERS_COMPILED) $(NAME)

debug: CPPFLAGS = $(BASE_CPPFLAGS) $(DEBUG_FLAGS)
debug: clean $(SHADERS_COMPILED) $(NAME)

run: all
	./$(NAME) ./examples/teapot.obj

rundebug: debug
	./$(NAME) ./examples/teapot.obj

rerun: fclean run

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(CPPFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LDFLAGS) $(LIBS)

%.spv : %
	$(GLSLC) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/*/%.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(SHADERS_COMPILED)

re: fclean all

.PHONY: all debug clean fclean re run rundebug rerun