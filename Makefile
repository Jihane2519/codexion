NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRC = src/main.c \
	src/parser.c \
	src/init.c \
	src/coder.c
OBJ = $(SRC:.c=.o)
all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

