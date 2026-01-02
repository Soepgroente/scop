NAME			:=	scop
CC				:=	c++
BASE_CPPFLAGS	:=	-Wall -Wextra -Werror -std=c++20 -fPIC
RELEASE_FLAGS	:=	-DNDEBUG -flto -O3 -march=native -fno-math-errno
DEBUG_FLAGS		:=	-g -fsanitize=address

INCLUDES	:=	-I./src \
				-I/opt/homebrew/include \
				-I./src/include \
				-I./src/vectors \
				-I./src/vulkan \

VECTOR_DIR	:=	src/vectors
LIBS		:=	$(VECTOR_DIR)/customVectors.a

SRCS	:=	Scop.cpp \
			include/stb_image.cpp \
			vulkan/Camera.cpp \
			vulkan/KeyboardInput.cpp \
			vulkan/MouseInput.cpp \
			vulkan/VulkanBuffer.cpp \
			vulkan/VulkanDevice.cpp \
			vulkan/VulkanDescriptors.cpp \
			vulkan/VulkanModel.cpp \
			vulkan/VulkanObjFileParser.cpp \
			vulkan/VulkanObject.cpp \
			vulkan/VulkanPipeline.cpp \
			vulkan/VulkanRenderer.cpp \
			vulkan/VulkanRenderSystem.cpp \
			vulkan/VulkanSwapChain.cpp \
			vulkan/VulkanTexture.cpp \
			vulkan/VulkanWindow.cpp \
			utils.cpp \

MAIN		:=	main.cpp

SRCDIR		:=	src
OBJDIR		:=	$(SRCDIR)/obj
OBJS		:=	$(addprefix $(OBJDIR)/,$(notdir $(SRCS:%.cpp=%.o)))
MAINOBJ		:=	$(OBJDIR)/$(notdir $(MAIN:%.cpp=%.o))

UNAME_S	:=	$(shell uname -s)

SHADERS_SRC	:=	src/shaders/shadyBusiness.vert \
				src/shaders/shadyBusiness.frag \

GLSLC				:= $(shell which glslc)
# source /opt/vulkan/current/setup-env.sh
SHADERS_COMPILED	:= $(SHADERS_SRC:%=%.spv)

LFLAGS		=	-L/opt/homebrew/lib -lglfw -framework Cocoa -framework IOKit -framework OpenGL
RPATH_DIR	:=	/usr/local/lib
LDFLAGS		:=	-lvulkan -lm -Wl,-rpath,$(RPATH_DIR)

ifeq ($(UNAME_S), Linux)
	ECHO_MESSAGE = "Linux"	
	INCLUDES += -isystem $(USER)/.capt/root/usr/include
	LFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl `pkg-config --static --libs glfw3`
	BASE_CPPFLAGS += `pkg-config --cflags glfw3`
endif

CPPFLAGS = $(BASE_CPPFLAGS) $(RELEASE_FLAGS)

all: $(SHADERS_COMPILED) $(NAME)

test:
	$(MAKE) -C $(VECTOR_DIR) test

retest: fclean test

debug: CPPFLAGS = $(BASE_CPPFLAGS) $(DEBUG_FLAGS)
debug: $(SHADERS_COMPILED) $(NAME)
debug:
	$(MAKE) -C $(VECTOR_DIR) debug

run: all
	./$(NAME) ./models/teapot.obj

rundebug: debug
	./$(NAME) ./models/teapot.obj

rerundebug: fclean rundebug

rerun: fclean run

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(LIBS):
	$(MAKE) -C $(VECTOR_DIR)

$(NAME): $(LIBS) $(OBJDIR) $(OBJS) $(MAINOBJ)
	$(CC) $(CPPFLAGS) $(INCLUDES) -o $(NAME) $(MAINOBJ) $(OBJS) $(LIBS) $(LDFLAGS) $(LFLAGS)

%.spv : %
	$(GLSLC) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/*/%.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	$(MAKE) -C $(VECTOR_DIR) clean

fclean:
	$(MAKE) -C $(VECTOR_DIR) fclean
	rm -rf $(OBJDIR)
	rm -f $(NAME)
	rm -f $(SHADERS_COMPILED)

re: fclean
	$(MAKE) -C $(VECTOR_DIR)
	$(MAKE) all

.PHONY: all debug clean fclean re run rundebug rerun