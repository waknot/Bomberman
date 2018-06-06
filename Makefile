CC 	= gcc

NAME 	= BomberMan

CFLAGS 	= -W -Werror -Wextra -Wall -Iincludes/ -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lpthread

SRC 	= main.c			\
	  base_map/base_map_manager.c   \
	  base_map/draw_base_map.c	\
	  base_map/init_tools.c		\

OBJ 	= ${SRC:%.c=%.o}

RM 	= rm -rf

all 	: ${NAME}

re 	: clean all

$(NAME) : $(OBJ)
	  $(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean	:
	  $(RM) $(OBJ)

fclean 	: clean
	  $(RM) $(NAME)

.PHONY	: all clean fclean
