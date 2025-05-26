# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/13 10:30:26 by trpham            #+#    #+#              #
#    Updated: 2025/05/26 13:37:36 by trpham           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc 
CFLAGS = -g -Wall -Wextra -Werror

SRCS_DIR = ./srcs/
SRCS = $(SRCS_DIR)/main.c \
		$(SRCS_DIR)/parsing/parser.c \
		$(SRCS_DIR)/parsing/token_linked_list.c \
		$(SRCS_DIR)/parsing/tokenization.c \
		$(SRCS_DIR)/parsing/helper.c \
		$(SRCS_DIR)/parsing/validate_input.c \
		$(SRCS_DIR)/parsing/redirection.c \
		$(SRCS_DIR)/parsing/history.c \
		$(SRCS_DIR)/parsing/expansion.c \
		$(SRCS_DIR)/ast/ast.c \
		$(SRCS_DIR)/heredoc/process_heredoc.c \
		$(SRCS_DIR)/heredoc/utils_heredoc.c \
		$(SRCS_DIR)/heredoc/utils_heredoc2.c \
		$(SRCS_DIR)/execution/execute_ast.c \
		$(SRCS_DIR)/execution/execute_command.c \
		$(SRCS_DIR)/execution/execute_pipe.c \
		$(SRCS_DIR)/execution/execute_redirection.c \
		$(SRCS_DIR)/execution/find_path.c \
		$(SRCS_DIR)/execution/utils.c \
		$(SRCS_DIR)/environment/env.c \
		$(SRCS_DIR)/builtin/builtin_cd.c \
		$(SRCS_DIR)/builtin/builtin_echo.c \
		$(SRCS_DIR)/builtin/builtin_env.c \
		$(SRCS_DIR)/builtin/builtin_pwd.c \
		$(SRCS_DIR)/builtin/builtin_unset.c \
		$(SRCS_DIR)/signals/handle_signal.c \
		$(SRCS_DIR)/shell/shell.c

OBJS = $(SRCS:%.c=%.o)

NAME = minishell

LIBFT_DIR = ./libft
LIBFT_NAME = $(LIBFT_DIR)/libft.a

HEADERS_DIR = ./includes
HEADERS = $(HEADERS_DIR)/shell.h

LDFLAGS = -lreadline
	
all: $(NAME)

INCLUDES = -I$(LIBFT_DIR)

%.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT_NAME)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "minishell is built successfully"

$(LIBFT_NAME):
	@make -C $(LIBFT_DIR)
	@echo "libft.a is built successfully"

clean:
	@make clean -C $(LIBFT_DIR)
	@rm -f $(OBJS)
	@echo "remove objects successfully"

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	rm -f  $(NAME)
	@echo "remove libft.a and minishell"

re: fclean all

.PHONY: all clean fclean re
