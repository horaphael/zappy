##
## EPITECH PROJECT, 2025
## network_lib
## File description:
## Makefile
##

CC		= 	gcc
CFLAGS	= 	-Wall -Wextra -Werror -Iinclude

SRC		= 	$(shell find src/ -type f -name "*.c")
OBJ 	= 	$(SRC:.c=.o)

LIBNAME = 	libnetwork.a
NAME	=	server

all: $(LIBNAME)

$(LIBNAME): $(OBJ)
	ar rcs $@ $^
	gcc main.c $(LIBNAME) $(CFLAGS) -o $(NAME)

clean:
	rm -f $(OBJ) *~ *# a.out

fclean: clean
	rm -f $(LIBNAME) $(NAME)

re: fclean all

.PHONY: all clean fclean re
