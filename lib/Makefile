NAME := lib.a

CC := cc
CFLAGS := -g -Wall -Wextra -Werror

LIBFT_DIR := 	./libft

FT_PRINTF_DIR := ./ft_printf_fd

GNL_DIR	:=	./get_next_line

LIBFT_SRC := 	$(LIBFT_DIR)/ft_atoi.c \
				$(LIBFT_DIR)/ft_bzero.c \
				$(LIBFT_DIR)/ft_calloc.c \
				$(LIBFT_DIR)/ft_isalnum.c \
				$(LIBFT_DIR)/ft_isalpha.c \
				$(LIBFT_DIR)/ft_isascii.c \
				$(LIBFT_DIR)/ft_isdigit.c \
				$(LIBFT_DIR)/ft_isprint.c \
				$(LIBFT_DIR)/ft_itoa.c \
				$(LIBFT_DIR)/ft_memchr.c \
				$(LIBFT_DIR)/ft_memcmp.c \
				$(LIBFT_DIR)/ft_memcpy.c \
				$(LIBFT_DIR)/ft_memmove.c \
				$(LIBFT_DIR)/ft_memset.c \
				$(LIBFT_DIR)/ft_putchar_fd.c \
				$(LIBFT_DIR)/ft_putendl_fd.c \
				$(LIBFT_DIR)/ft_putnbr_fd.c \
				$(LIBFT_DIR)/ft_putstr_fd.c \
				$(LIBFT_DIR)/ft_split.c \
				$(LIBFT_DIR)/ft_strchr.c \
				$(LIBFT_DIR)/ft_strdup.c \
				$(LIBFT_DIR)/ft_striteri.c \
				$(LIBFT_DIR)/ft_strjoin.c \
				$(LIBFT_DIR)/ft_strlcat.c \
				$(LIBFT_DIR)/ft_strlcpy.c \
				$(LIBFT_DIR)/ft_strlen.c \
				$(LIBFT_DIR)/ft_strmapi.c \
				$(LIBFT_DIR)/ft_strncmp.c \
				$(LIBFT_DIR)/ft_strnstr.c \
				$(LIBFT_DIR)/ft_strrchr.c \
				$(LIBFT_DIR)/ft_strtrim.c \
				$(LIBFT_DIR)/ft_substr.c \
				$(LIBFT_DIR)/ft_tolower.c \
				$(LIBFT_DIR)/ft_toupper.c \
				$(LIBFT_DIR)/ft_lstadd_back_bonus.c \
				$(LIBFT_DIR)/ft_lstadd_front_bonus.c  \
				$(LIBFT_DIR)/ft_lstclear_bonus.c \
				$(LIBFT_DIR)/ft_lstdelone_bonus.c \
				$(LIBFT_DIR)/ft_lstiter_bonus.c \
				$(LIBFT_DIR)/ft_lstlast_bonus.c \
				$(LIBFT_DIR)/ft_lstmap_bonus.c \
				$(LIBFT_DIR)/ft_lstnew_bonus.c	\
				$(LIBFT_DIR)/ft_lstsize_bonus.c \
				$(LIBFT_DIR)/ft_strcmp.c \

FT_PRINTF_SRC	:=	$(FT_PRINTF_DIR)/ft_putchar.c \
					$(FT_PRINTF_DIR)/ft_putstr.c \
					$(FT_PRINTF_DIR)/ft_putnbr_base.c \
					$(FT_PRINTF_DIR)/ft_putptr.c \
					$(FT_PRINTF_DIR)/ft_putnbr.c \
					$(FT_PRINTF_DIR)/ft_putnbr_unsigned.c \
					$(FT_PRINTF_DIR)/ft_printf_fd.c


GNL_SRC	:=	$(GNL_DIR)/get_next_line.c	\
			$(GNL_DIR)/get_next_line_utils.c

OBJ := $(LIBFT_SRC:.c=.o) $(FT_PRINTF_SRC:.c=.o) $(GNL_SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Building $(NAME)..."
	@ar rcs $(NAME) $(OBJ)
	@echo "Created $(NAME)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJ)

fclean: clean
	@echo "Removing $(NAME)..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re