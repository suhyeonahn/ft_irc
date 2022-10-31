INC_DIR		=	./include/

SRCS		=	./src/main.cpp \
				./src/utils/Utils.cpp \
				./src/utils/DEBUG.cpp \
				./src/server/Client.cpp \
				./src/server/Server.cpp \
				./src/irc/IRC.cpp \
				./src/irc/User.cpp \
				./src/irc/Cmd.cpp \
				./src/irc/Channel.cpp \
				./src/irc/Numerics.cpp \
				./src/irc/ParseCmd.cpp \
				./src/irc/cmds/INVITE.cpp \
				./src/irc/cmds/JOIN.cpp \
				./src/irc/cmds/KICK.cpp \
				./src/irc/cmds/LIST.cpp \
				./src/irc/cmds/MODE.cpp \
				./src/irc/cmds/NAMES.cpp \
				./src/irc/cmds/NICK.cpp \
				./src/irc/cmds/NOTICE.cpp \
				./src/irc/cmds/PART.cpp \
				./src/irc/cmds/PASS.cpp \
				./src/irc/cmds/PRIVMSG.cpp \
				./src/irc/cmds/QUIT.cpp \
				./src/irc/cmds/TOPIC.cpp \
				./src/irc/cmds/USER.cpp \
				./src/irc/cmds/WHO.cpp \

OBJS		= $(SRCS:.cpp=.o)

CC			=	clang++

CFLAGS		=	-Wall -Wextra -Werror -g3 -fsanitize=address  -std=c++98 

NAME		=	ircserv

.cpp.o:
			@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $(<:.cpp=.o)
			@echo "\033[92m... \033[0m"

$(NAME):		$(OBJS)
			@echo "\033[92m\n\nObject files has been compiled!\n \033[0m"
			@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
			@echo "\033[94m\n$(NAME) is up to ready.\n\nCommand with: ./ircserv <port> <password>\n\n\033[0m"


all:			$(NAME)

clean:
			@rm -rf $(OBJS)
			@echo "\033[91m\nObject files has been removed.\033[0m"

fclean:			clean
			@rm -f $(NAME)
			@echo "\033[91m\nExecutable file has been removed.\n\033[0m"

re: 		fclean all


.PHONY:		all clean fclean re bonus
