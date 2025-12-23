NAME			:=	scop
CC				:=	c++
BASE_CPPFLAGS	=	-Wall -Wextra -Werror -std=c++20 -fPIC
RELEASE_FLAGS	:=	-DNDEBUG -flto -O3 -march=native
DEBUG_FLAGS		:=	-g -fsanitize=address

INCLUDES	=	-I./src \
				-I/opt/homebrew/include \
				-I./src/include \
				-I./src/vectors \
				-I./src/vulkan \

SRCS	:=	Scop.cpp \
			vectors/Mat3.cpp \
			vectors/Mat4.cpp \
			vectors/Quat.cpp \
			vectors/Vec2.cpp \
			vectors/Vec3.cpp \
			vectors/Vec4.cpp \
			vectors/Vectors.cpp \
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

TEST_EXEC	:=	testmath

TEST_SRCS	:=	matrixTests.cpp \
				quaternionTests.cpp \
				testmain.cpp \
				vectorTests.cpp \

TEST_DIR	:=	./tests

# ifeq ($(shell uname), Darwin)
# 	CPUCORES := $(shell sysctl -n hw.ncpu)
# else
# 	CPUCORES := $(shell nproc)
# endif
# MAKEFLAGS	+= -j$(CPUCORES)
# export MAKEFLAGS

SRCDIR		:=	src
OBJDIR		:=	$(SRCDIR)/obj
OBJS		:=	$(addprefix $(OBJDIR)/,$(notdir $(SRCS:%.cpp=%.o)))
TEST_OBJS	:=	$(addprefix $(OBJDIR)/,$(notdir $(TEST_SRCS:%.cpp=%.o)))

MAINOBJ		:=	$(OBJDIR)/$(notdir $(MAIN:%.cpp=%.o))

UNAME_S	:=	$(shell uname -s)

SHADERS_SRC	:=	src/shaders/shadyBusiness.vert \
				src/shaders/shadyBusiness.frag \

GLSLC				:= $(shell which glslc)
# source /opt/vulkan/current/setup-env.sh
SHADERS_COMPILED	:= $(SHADERS_SRC:%=%.spv)

LIBS		=	-L/opt/homebrew/lib -lglfw -framework Cocoa -framework IOKit -framework OpenGL
RPATH_DIR	:=	/usr/local/lib
LDFLAGS		:=	-lvulkan -lm -Wl,-rpath,$(RPATH_DIR)

ifeq ($(UNAME_S), Linux)
	ECHO_MESSAGE = "Linux"	
	INCLUDES += -isystem $(USER)/.capt/root/usr/include
	LIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl `pkg-config --static --libs glfw3`
	BASE_CPPFLAGS += `pkg-config --cflags glfw3`
endif

CPPFLAGS = $(BASE_CPPFLAGS) $(RELEASE_FLAGS)

all: $(SHADERS_COMPILED) $(NAME)

test: $(TEST_EXEC)
	./$(TEST_EXEC) || true

retest: fclean test

debug: CPPFLAGS = $(BASE_CPPFLAGS) $(DEBUG_FLAGS)
debug: $(SHADERS_COMPILED) $(NAME)

run: all
	./$(NAME) ./models/teapot.obj

rundebug: debug
	./$(NAME) ./models/teapot.obj

rerundebug: fclean rundebug

rerun: fclean run

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJS) $(MAINOBJ)
	$(CC) $(CPPFLAGS) $(INCLUDES) -o $(NAME) $(MAINOBJ) $(OBJS) $(LDFLAGS) $(LIBS)

$(TEST_EXEC): $(OBJDIR) $(TEST_OBJS) $(OBJS)
	$(CC) $(CPPFLAGS) $(INCLUDES) -I$(TEST_DIR) -o $(TEST_EXEC) $(OBJS) $(TEST_OBJS)

%.spv : %
	$(GLSLC) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/*/%.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(TEST_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -I$(TEST_DIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(SHADERS_COMPILED)

re: fclean all

.PHONY: all debug clean fclean re run rundebug rerun