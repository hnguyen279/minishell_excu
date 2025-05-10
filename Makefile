NAME 			:= minishell

CC 				:= cc
CFLAGS 			:= -Wall -Wextra -Werror

LIB_DIR			:= ./lib
LIB				:= $(LIB_DIR)/lib.a

INCLUDES			:= -I $(LIB_DIR) -I ./includes

MAN_DIR			:= ./srcs
MAN_FILES		:= 

MAN_SRC			:= main.c $(addprefix $(MAN_DIR)/, $(MAN_FILES))

MAN_OBJ			:= $(MAN_SRC:.c=.o)

all: $(NAME)

$(NAME): $(MAN_OBJ) $(LIB)
	$(CC) $(MAN_OBJ) $(LIB) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(LIB):
	@make -C $(LIB_DIR)

clean:
	@echo "Cleaning object files..."
	@rm -f $(MAN_OBJ)
	@make -C $(LIB_DIR) clean

fclean:
	@echo "Removing all compiled files..."
	@rm -f $(MAN_OBJ) $(NAME)
	@make -C $(LIB_DIR) fclean
	
re: fclean all

.PHONY: all clean fclean re
