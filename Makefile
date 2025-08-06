NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDLIBS = -lreadline

# Directories
SRC_DIR = src
BUILTIN_DIR = $(SRC_DIR)/builtins
MINISHELL_DIR = $(SRC_DIR)/minishell
PARSER_DIR = $(SRC_DIR)/parser
PIPES_DIR = $(SRC_DIR)/pipes
REDIRECT_DIR = $(SRC_DIR)/redirections
SIGNAL_DIR = $(SRC_DIR)/signal
TOKEN_DIR = $(SRC_DIR)/token
INCLUDE_DIR = include
LIBFT_DIR = libft

# Source files
BUILTIN_SRCS = $(BUILTIN_DIR)/builtins.c $(BUILTIN_DIR)/echo.c $(BUILTIN_DIR)/pwd.c $(BUILTIN_DIR)/cd.c $(BUILTIN_DIR)/env.c $(BUILTIN_DIR)/exit.c $(BUILTIN_DIR)/export.c $(BUILTIN_DIR)/unset.c $(BUILTIN_DIR)/env_utils.c $(BUILTIN_DIR)/env_expansion.c
MINISHELL_SRCS = $(MINISHELL_DIR)/minishell.c $(MINISHELL_DIR)/minishell_utils.c $(MINISHELL_DIR)/minishell_exec.c
PARSER_SRCS = $(PARSER_DIR)/parser.c $(PARSER_DIR)/parser_utils.c
PIPES_SRCS = $(PIPES_DIR)/pipes.c $(PIPES_DIR)/pipes_utils.c $(PIPES_DIR)/pipes_redirection.c $(PIPES_DIR)/pipes_execution.c
REDIRECT_SRCS = $(REDIRECT_DIR)/redirections.c $(REDIRECT_DIR)/redirections_utils.c
SIGNAL_SRCS = $(SIGNAL_DIR)/signals.c
TOKEN_SRCS = $(TOKEN_DIR)/token.c $(TOKEN_DIR)/token_utils.c

SRCS = $(BUILTIN_SRCS) $(MINISHELL_SRCS) $(PARSER_SRCS) $(PIPES_SRCS) $(REDIRECT_SRCS) $(SIGNAL_SRCS) $(TOKEN_SRCS)
OBJS = $(SRCS:.c=.o)

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a
INCLUDES = -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -I$(LIBFT_DIR)/get_next_line

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) bonus

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDLIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
