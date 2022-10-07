NAME_SERVER = server

NAME_CLIENT = client

SRC_SERVER = server.c

SRC_CLIENT = client.c

OBJ_SERVER = $(SRC_SERVER:.c=.o)

OBJ_CLIENT = $(SRC_CLIENT:.c=.o)

CFLAGS = -Wall -Wextra -Werror
all	: $(NAME_SERVER) $(NAME_CLIENT)

$(NAME_SERVER) : $(OBJ_SERVER)
	gcc -pthread -o $(NAME_SERVER) $(SRC_SERVER) $(CFLAGS)

$(NAME_CLIENT) : $(OBJ_CLIENT)
	gcc -pthread -o $(NAME_CLIENT) $(SRC_CLIENT) $(CFLAGS)

clean	:
	rm -rf $(OBJ_SERVER) $(OBJ_CLIENT)

fclean	: clean
	rm -rf $(NAME_SERVER) $(NAME_CLIENT)

re : fclean 
	make all

-include $(DEPS)


.PHONY : re clean fclean all