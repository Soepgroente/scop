NAME	:=	scop
CC		:=	c++
CFLAGS	:=	-Wall -Wextra -Werror -std=c++20
INCLUDES:=	-I./src -I./libs/glm \
			-I/opt/homebrew/include \
			-I./src/ObjectClasses \
			-I./src/VectorClasses \
			-I./src/vulkan \
			-I./src \

SRCS	:=	main.cpp \
			ObjectClasses/Camera.cpp \
			ObjectClasses/Material.cpp \
			ObjectClasses/Object.cpp \
			ObjectClasses/ObjectParser.cpp \
			ObjectClasses/Scene.cpp \
			Scop.cpp \
			VectorClasses/Vec3.cpp \
			VectorClasses/Vec4.cpp \
			vulkan/VulkanDevice.cpp \
			vulkan/VulkanPipeline.cpp \
			vulkan/VulkanSwapChain.cpp \
			vulkan/VulkanWindow.cpp \

SHADERS_SRC	:=	src/shaders/shadyBusiness.vert \
				src/shaders/shadyBusiness.frag \

SHADER_COMPILE_CMD := $(shell which glslc)

SRCDIR	:=	src
OBJDIR	:=	$(SRCDIR)/obj

OBJS	:=	$(addprefix $(OBJDIR)/,$(notdir $(SRCS:%.cpp=%.o)))

SHADERS_COMPILED := $(SHADERS_SRC:%=%.spv)

LIBS	:=	-L/opt/homebrew/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit

RPATH_DIR	:=	/usr/local/lib
LDFLAGS		:=	-lvulkan -I/opt/homebrew/include -lm
LDFLAGS		+=	-Wl -rpath $(RPATH_DIR)

ifeq ($(UNAME_S), Linux)
	ECHO_MESSAGE = "Linux"
	LIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl `pkg-config --static --libs glfw3`

	CPPFLAGS += `pkg-config --CPPFLAGS glfw3`
endif

all: $(SHADERS_COMPILED) $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LDFLAGS) $(LIBS)

%.frag.spv : %.frag
	$(SHADER_COMPILE_CMD) $< -o $@

%.vert.spv : %.vert
	$(SHADER_COMPILE_CMD) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/*/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(SHADERS_COMPILED)

re: fclean all

.PHONY: all clean fclean re
