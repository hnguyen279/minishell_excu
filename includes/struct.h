#ifndef SHELL_H
# define SHELL_H

# include "../libft/includes/libft.h"
# include <signal.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>

# define TRUE 0
# define FALSE -1
# define FD_READ 0
# define FD_WRITE 1
# define MODE_INTERACTIVE 0
# define MODE_HEREDOC 1
# define MODE_CHILD 2

extern volatile sig_atomic_t g_signum;

typedef enum e_token_type
{
    NONE = 0,
    PIPE = 1,
    WORD = 2,
    REDIR_IN = 3, //>
    REDIR_OUT = 4, //<
    REDIR_APPEND = 5, // >>
    REDIR_HEREDOC = 6 // <<
} t_token_type;

typedef enum e_error_type
{
    ERR_NONE = 0,
    ERR_MALLOC = 1,
    ERR_QUOTE = 2,
    ERR_PIPE = 3,
    ERR_REDIR = 4,
    ERR_SYNTAX = 5,
    ERR_FORK = 6
} t_error_type;

typedef struct s_token
{
    char *value;
    t_token_type type;
    struct s_token *next;
    struct s_token *prev;
} t_token;

typedef enum e_redirect_type
{
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} t_redirect_type;

typedef struct s_redirect
{
    int fd; // 0 for stdin, 1 for stdout
    t_redirect_type type; // REDIR_IN, REDIR_OUT, REDIR_APPEND, REDIR_HEREDOC
    char *file; // File name or heredoc delimiter
    char *ori_path; // Not used currently
    char *tmp_file; // Temporary file for heredoc
    struct s_redirect *next;
} t_redirect;

typedef struct s_cmd
{
    char *cmd_name;
    char **args;
    t_redirect *redirects;
    struct s_cmd *next;
} t_cmd;

typedef enum e_node_type
{
    NODE_NONE = 0,
    NODE_CMD,
    NODE_PIPE
} t_node_type;

typedef struct s_ast
{
    t_node_type type;
    char **cmd; // Array of command and arguments
    t_redirect *redirects;
    struct s_ast *left;
    struct s_ast *right;
} t_ast;

typedef struct s_tokens
{
    int has_pipe;
    char *env_last_cmd;
    char **token;
} t_tokens;

typedef struct s_shell
{
    int exit_code;
    char **envp;
    int heredoc_index;
    t_ast *ast;
    t_tokens *tokens;
} t_shell;


#endif