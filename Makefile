##
## EPITECH PROJECT, 2025
## B-YEP-400-RUN-4-1-zappy-julien.kbidi
## File description:
## Makefile
##

CXX			=	g++
CC			=	gcc
CXXFLAGS	=	-Wall -Wextra -Iincludes/GUI
CFLAGS		=	-Wall -Wextra -Werror -Iincludes/Server -lpthread -g3
LDFLAGS		= 	-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

PYTHON		=	python3

SERVER_SRC	= 	$(shell find src/Server/ -type f -name "*.c")
GUI_SRC		= 	$(shell find src/GUI/ -type f -name "*.cpp")
AI_SRC		= 	src/AI/main.py

SERVER_OBJ	= 	$(SERVER_SRC:.c=.o)
GUI_OBJ		= 	$(GUI_SRC:.cpp=.o)

SERVER_BIN	= 	zappy_server
GUI_BIN		= 	zappy_gui
AI_BIN		= 	zappy_ai

all: $(SERVER_BIN) $(GUI_BIN) $(AI_BIN)

zappy_server: $(SERVER_OBJ)
	@printf "\033[1;36m[Compiling server...]\n\033[0m"
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_OBJ)

zappy_gui: $(GUI_OBJ)
	@printf "\033[1;35m[Compiling GUI...]\n\033[0m"
	@printf "\033[1;35m["
	$(CXX) $(CXXFLAGS) -o $(GUI_BIN) $(GUI_OBJ) $(LDFLAGS)
	@printf "]\n\033[0m"

zappy_ai: $(AI_SRC)
	@printf "\033[1;34m[Creating AI launcher...]\n\033[0m"
	@printf "\033[1;34m["
	@cp $(AI_SRC) $(AI_BIN)
	@printf "]\n\033[0m"
	@chmod +x $(AI_BIN)

clean:
	@printf "\033[1;31m[Removing object files...]\n\033[0m"
	@rm -f $(SERVER_OBJ) $(GUI_OBJ) *~ *# sys vgcore*

fclean: clean
	@printf "\033[1;31m[Removing binaries...]\n\033[0m"
	@rm -f $(SERVER_BIN) $(GUI_BIN) $(AI_BIN)

re: fclean all

.PHONY: all zappy_server zappy_gui zappy_ai clean fclean re
