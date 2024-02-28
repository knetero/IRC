CC = c++
FLAGS = -g -Wall -Wextra -Werror -std=c++98

SRCS = main.cpp server.cpp client.cpp
OBJS = $(SRCS:.cpp=.o)
NAME = ircserv
HEADERS = server.hpp  client.hpp

###############################
WHITE		=	"\033[m"
RED			=	"\033[1;31m"
GREEN		=	"\033[1;32m"
YELLOW		=	"\033[1;33m"
BLUE		=	"\033[1;34m"
PURPLE		=	"\033[1;35m"
CYAN		=	"\033[1;36m"
EOC			=	"\033[0;0m"
LINE_CLEAR	=	"\x1b[1A\x1b[M"
###############################

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	@echo $(GREEN) "Building $(NAME)..." $(EOC)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo $(GREEN) "$(NAME) is created!\n" $(EOC)

%.o: %.cpp $(HEADERS)
	@echo $(YELLOW) "Compiling...\t" $< $(EOC) $(LINE_CLEAR)
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@echo $(YELLOW) "Cleaning object files..." $(EOC)
	@$(RM) $(OBJS)
	@echo $(RED) "Object files are cleaned!" $(EOC)

fclean: clean
	@echo $(YELLOW) "\n Removing $(NAME)..." $(EOC)
	@$(RM) $(NAME)
	@echo $(RED) "$(NAME) is removed!" $(EOC)

re: fclean all
