# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/13 10:30:26 by trpham            #+#    #+#              #
#    Updated: 2025/06/13 10:20:42 by trpham           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc 
CFLAGS = -Wall -Wextra -Werror

SRCS_DIR = ./srcs/
SRCS = $(SRCS_DIR)/main.c \
		$(SRCS_DIR)/parsing/handle_input.c \
		$(SRCS_DIR)/parsing/handle_input2.c \
		$(SRCS_DIR)/parsing/handle_quote.c \
		$(SRCS_DIR)/parsing/token_list.c \
		$(SRCS_DIR)/parsing/tokenization.c \
		$(SRCS_DIR)/parsing/retokenization_split.c \
		$(SRCS_DIR)/parsing/helper.c \
		$(SRCS_DIR)/parsing/validate_input.c \
		$(SRCS_DIR)/parsing/validate_input2.c \
		$(SRCS_DIR)/parsing/history.c \
		$(SRCS_DIR)/parsing/expansion.c \
		$(SRCS_DIR)/parsing/free_function.c \
		$(SRCS_DIR)/parsing/free_function2.c \
		$(SRCS_DIR)/ast/ast.c \
		$(SRCS_DIR)/ast/cmd_list.c \
		$(SRCS_DIR)/ast/cmd_args.c \
		$(SRCS_DIR)/ast/redirects.c \
		$(SRCS_DIR)/heredoc/process_heredoc.c \
		$(SRCS_DIR)/heredoc/utils_and_heredoc_file.c \
		$(SRCS_DIR)/heredoc/prepare_delimiter.c \
		$(SRCS_DIR)/heredoc/heredoc_loop.c \
		$(SRCS_DIR)/execution/execute_ast.c \
		$(SRCS_DIR)/execution/execute_command.c \
		$(SRCS_DIR)/execution/execute_pipe.c \
		$(SRCS_DIR)/execution/execute_redirection.c \
		$(SRCS_DIR)/execution/execute_builtin.c \
		$(SRCS_DIR)/execution/find_path.c \
		$(SRCS_DIR)/execution/utils_exe.c \
		$(SRCS_DIR)/execution/utils_support_exe.c \
		$(SRCS_DIR)/execution/utils_redirection.c \
		$(SRCS_DIR)/environment/env_add.c \
		$(SRCS_DIR)/environment/utils_env.c \
      	$(SRCS_DIR)/environment/env_init.c \
      	$(SRCS_DIR)/environment/env_search.c \
      	$(SRCS_DIR)/environment/env_remove.c \
      	$(SRCS_DIR)/environment/env_last_argument.c \
		$(SRCS_DIR)/builtin/builtin_cd.c \
		$(SRCS_DIR)/builtin/builtin_echo.c \
		$(SRCS_DIR)/builtin/builtin_env.c \
		$(SRCS_DIR)/builtin/builtin_pwd.c \
		$(SRCS_DIR)/builtin/builtin_unset.c \
		$(SRCS_DIR)/builtin/builtin_exit.c \
		$(SRCS_DIR)/builtin/builtin_export.c \
		$(SRCS_DIR)/builtin/utils_export.c \
		$(SRCS_DIR)/builtin/utils_cd_export.c \
		$(SRCS_DIR)/signals/handle_signal.c \
		$(SRCS_DIR)/signals/utils_signal.c \
		$(SRCS_DIR)/shell/shell.c \
		$(SRCS_DIR)/shell/shell2.c \
		$(SRCS_DIR)/shell/user_input.c \
		$(SRCS_DIR)/parsing/to_delete_file.c \
		$(SRCS_DIR)/main_test.c

OBJS = $(SRCS:%.c=%.o)

NAME = minishell

LIBFT_DIR = ./libft
LIBFT_NAME = $(LIBFT_DIR)/libft.a

HEADERS_DIR = ./includes
HEADERS = $(HEADERS_DIR)/shell.h \
			$(HEADERS_DIR)/token.h \
			$(HEADERS_DIR)/parsing.h

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
