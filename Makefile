NAME	:=	scop
CC		:=	c++
CFLAGS	:=	-Wall -Wextra -Werror -std=c++20
INCLUDES:=	-I./src -I./libs/glm -I/opt/homebrew/include -I./src/ObjectClasses -I./src/VectorClasses

SRCS	:=	main.cpp \
			ObjectClasses/Camera.cpp \
			ObjectClasses/Material.cpp \
			ObjectClasses/Object.cpp \
			ObjectClasses/ObjectParser.cpp \
			ObjectClasses/Scene.cpp \
			ObjectClasses/WindowManager.cpp \
			VectorClasses/Vec3.cpp \
			VectorClasses/Vec4.cpp \

SRCDIR	:=	src
OBJDIR	:=	$(SRCDIR)/obj

OBJS	:=	$(addprefix $(OBJDIR)/,$(notdir $(SRCS:%.cpp=%.o)))

LIBS	:=	-L/opt/homebrew/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit 

ifeq ($(UNAME_S), Linux)
	ECHO_MESSAGE = "Linux"
	LIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl `pkg-config --static --libs glfw3`

	CPPFLAGS += `pkg-config --CPPFLAGS glfw3`
endif

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/*/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
