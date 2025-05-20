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
    REDIR_IN_token = 3, 
    REDIR_OUT_token = 4, 
    REDIR_APPEND_token = 5,
    REDIR_HEREDOC_token = 6
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
    REDIR_HEREDOC,
    REDIR_INVALID
} t_redirect_type;

typedef struct s_redirect
{
    int fd;
    t_redirect_type type;
    char *file;
    char *ori_path;
    char *tmp_file;
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
    char **cmd;
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
    int has_pipe;
    char *env_last_cmd;
    t_ast *ast;
} t_shell;

/* Shell initialization */
int init_shell(t_shell *mshell, char **envp);
void shell_cleanup(t_shell *mshell);

/* Tokenization */
int is_comment(char *s);
t_token *create_token(char *s, t_token_type i);
char *extract_quoted_token(char *line, int *i);
char *extract_word(char *line, int *i);
void add_token(t_token **tokenized_input_list, t_token *new_token);
t_token *convert_user_input_to_token(char *line);
//void free_token_list(t_token *tokens);

/* Validate input */
int validate_token(t_token *token);
int is_pipe(t_token *token);
int is_valid_pipe(t_token *token);
int is_redirection(t_token *token);
int is_valid_redirection(t_token *token_list);
int validate_quote(char *line);
int is_operator(t_token *token);

/* Parsing */
t_cmd   *parse_tokens_to_commands(t_token *tokenized_input_list);
t_cmd *create_cmd(void);
int update_command_node(t_cmd **new_cmd, t_token **temp_token_list);
int count_args(t_token *tokenized_input_list);
char **fill_args(t_token **token_list);
int add_redirects(t_redirect **redir_list, t_redirect_type type, char *file);
t_redirect_type token_to_redirect_type(t_token_type token_type);  
int parse_redirection(t_cmd **new_cmd, t_token **token_list);
void print_cmd_list(t_cmd *head);
void print_redirect_list(t_redirect *redir_list);
void free_cmd_list(t_cmd *head);

/* Abstract Syntax Tree */
t_ast *convert_cmd_to_ast(t_cmd *cmd_list);
void free_ast(t_ast *node, t_shell *mshell);

/* Working history functions */
void store_history(char *line, t_token **history_head);
void print_working_history(t_token *history_head);
void clear_working_history(t_token **history_head);

/* Helper functions */
void free_string(char *s);
void free_array(char **arr, int i);
int array_size(char **arr);
void print_linked_list(t_token *head);
void print_error(char *msg);
void get_error_msg(t_error_type err);
int ft_isspace(char c);
void print_array(char **arr);

/* Heredoc functions */
int open_heredoc_pipe(t_shell *mshell, t_redirect *redir);
void exe_handle_dollar_expansion(char *input, int fd_write, t_shell *ms);
int inside_quotes(const char *str);
char *get_delimiter(char *file);
char *make_heredoc_filename_from_fd(int fd);
int process_heredocs(t_shell *mshell, t_ast *node);

/* Execution functions */
void free_split(char **tab);
int display_error(char *cmd);
int check_white_spaces_cmd(char *cmd);
int check_ambiguous_redir(t_shell *mshell, t_redirect *redir);
char *find_cmd_path(t_shell *mshell, char *cmd);
int exe_redirection(t_redirect *redir, t_shell *mshell);
int execute_pipe(t_ast *ast, t_shell *shell);
int execute_command(t_ast *node, t_shell *mshell);
int execute_ast(t_ast *node, t_shell *mshell);

/* Env functions */
int init_env(t_shell *mshell, char **envp);
void env_free(t_shell *mshell);
char **env_dup(char **envp);
char *env_find_variable(t_shell *mshell, const char *key, size_t *i);
char *env_find_value(t_shell *mshell, const char *key);
int env_add(t_shell *mshell, const char *key, const char *value);
int env_remove(t_shell *mshell, const char *key);
void env_sort(char **envp, size_t len);
void env_swap_last(char **envp);
void env_print(char **envp);
int env_underscore(t_shell *mshell, char **cmd);
int env_clone_underscore(t_shell *mshell);
void ft_free_null(char ***array);

/* Built-in functions */
int builtin_cd(t_shell *mshell, char **token);
int builtin_echo(t_shell *mshell, char **token);
int builtin_env(t_shell *mshell, char **token);
int builtin_pwd(t_shell *mshell, char **token);
int builtin_unset(t_shell *mshell, char **token);

/* Signal functions */
void set_sigint_flag(int sig);
int reset_shell_prompt(void);
int reset_heredoc_prompt(void);
int signal_error(t_shell *mshell, const char *message);
int setup_signal_handlers(t_shell *mshell, void (*sigint_handler)(int), void (*sigquit_handler)(int));
void setup_signals(t_shell *mshell, int mode);
void sig_exit_code(t_shell *mshell);

#endif