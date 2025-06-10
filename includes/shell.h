/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 06:09:47 by trpham            #+#    #+#             */
/*   Updated: 2025/06/10 22:12:39 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "../libft/includes/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h>

# define TRUE 0
# define FALSE -1
# define FD_READ 0
# define FD_WRITE 1
# define MODE_INTERACTIVE 0
# define MODE_HEREDOC 1
# define MODE_CHILD 2

extern volatile sig_atomic_t	g_signum;

typedef enum e_token_type
{
	NONE = 0,
	PIPE = 1,
	WORD = 2,
	REDIR_IN_TOKEN = 3,
	REDIR_OUT_TOKEN = 4,
	REDIR_APPEND_TOKEN = 5,
	REDIR_HEREDOC_TOKEN = 6
}	t_token_type;

typedef enum e_error_type
{
	ERR_NONE = 0,
	ERR_MALLOC = 1,
	ERR_QUOTE = 2,
	ERR_PIPE = 3,
	ERR_REDIR = 4,
	ERR_SYNTAX = 5,
	ERR_FORK = 6
}	t_error_type;

typedef struct s_token
{
	char				*value;
	char				*ori_value;
	t_token_type		type;
	struct s_token		*next;
	struct s_token		*prev;
	// int					in_single_quote; // true if in and false if not
	// int					in_double_quote;
	// int					is_expansion; //0 not expansion, 1 is expansion from var
}	t_token;

typedef enum e_redirect_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	REDIR_INVALID
}	t_redirect_type;

typedef struct s_redirect
{
	int					fd;
	t_redirect_type		type;
	char				*file;
	char				*ori_file;
	char				*tmp_file;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_cmd
{
	char				*cmd_name;
	char				**args;
	t_redirect			*redirects;
	struct s_cmd		*next;
}	t_cmd;

typedef enum e_node_type
{
	NODE_NONE = 0,
	NODE_CMD,
	NODE_PIPE
}	t_node_type;

typedef struct s_ast
{
	t_node_type			type;
	char				**cmd;
	t_redirect			*redirects;
	struct s_ast		*left;
	struct s_ast		*right;
}	t_ast;

typedef struct s_shell
{
	int					exit_code;
	char				**envp;
	int					heredoc_index;
	int					has_pipe;
	t_ast				*ast;
}	t_shell;

/* Shell initialization */
int				init_shell(t_shell *mshell, char **envp);
void			shell_interactive(t_shell *mshell);
void			shell_cleanup(t_shell *mshell);

/* Handle user input */
int				handle_special_command_line(char *line, t_token **history_head);
void			handle_line(char *line, t_shell *mshell);
int				init_and_validate_input(char *line, t_shell *mshell,
					t_token **token_list);
void			process_valid_line(t_shell *mshell,	t_token **oken_list,
					t_cmd **cmd_list, t_ast **tree);
void			run_ast_pipeline(t_shell *mshell, t_ast *tree);

/* Tokenization */
int				tokenization_expansion_validation(char *line, t_shell *mshell,
					t_token **token_list);
// int				empty_variable_extension(t_shell *mshell, t_token **token_list);
// int				empty_variable_skip(t_shell *mshell, t_token **token_list);
int				skip_expanded_empty_var(t_token **token_list);
void			skip_first_empty_vars(t_token **token_list);
void			skip_middle_empty_vars(t_token **token_list);
int 			echo_with_n(t_token *token_list);

// t_token			*create_token(char *s, t_token_type i);
t_token			*create_token(char *s, char *ori_s, t_token_type i);
int				handle_pipe(t_token **token_list, int *i);
int				handle_in_heredoc(char *line, t_token **token_list, int *i);
int				handle_out_append(char *line, t_token **token_list, int *i);
// char			*extract_quoted_token(char *line, int *i);
void			add_token(t_token **tokenized_input_list, t_token *new_token);
t_token			*convert_user_input_to_token(char *line, t_shell *mshell);
int				handle_word(char *line, t_token **token_list, int *i,
					t_shell *mshell);
char			*extract_ori_word(char *line, int *i);
//char	*extract_original_value_with_quote(char *line, int *index);
//char	*extract_original_without_quote(char *line, int *index);

char			*extract_full_word(char *line, int *i, t_shell *mshell);
char			*handle_single_quote(char *line, int *i);
char			*handle_double_quote(char *line, int *i, t_shell *mshell);
char			*extract_unquoted_word(char *line, int *i, t_shell *mshell);
// int				empty_single_quote_check(char **part, int *i, int start_pos);
// int				empty_double_quote_check(char **part, int *i, int start_pos);
int				substr_and_move_index(char *line, char **part, int *i,
					int start_pos);

/* Expansion token */
char			*expand_token_value(char *str, t_shell	*mshell);
void			handle_dollar_sign(char *str, t_shell *mshell, char **result,
					int *i);
char			*handle_env_variable(char **str, t_shell *mshell, int *i,
					char *result);
char			*expand_exit_code(t_shell *mshell, char	*result, int *i);
char			*extract_variable_name(char **str, char **var_name, int *i,
					char **result);

/* Validate input */
int				validate_token(t_token *token);
int				is_pipe(t_token *token);
int				is_valid_pipe(t_token *token);
int				is_redirection(t_token *token);
int				is_valid_redirection(t_token *token_list);
int				validate_quote(char *line);
int				is_operator(t_token *token);
// int				is_valid_parentheses(t_token *token_list);
// int				validate_parentheses_pair(char *line);

/* Parsing */
t_cmd			*parse_tokens_to_commands(t_token *tokenized_list);
t_cmd			*create_cmd(void);
int				update_command_node(t_cmd **new_cmd, t_token **temp_token_list);
int				add_and_update_cmd_node(t_token **temp_token_list,
					t_cmd **cmd_list);
int				count_args(t_token *tokenized_input_list);
char			**fill_args(t_cmd **new_cmd, t_token **token_list);
char			**allocate_arg_array(int count);
int				fill_args_loop(t_token **token_list, char **args,
					t_cmd **new_cmd);
// int				add_redirects(t_redirect **redir_list, t_redirect_type type,
// 					char *file);
// int				check_redir_type_before_parsing(t_cmd **new_cmd,
// 					t_token **token_list, t_redirect_type *redir_type);
int				check_redir_type_before_parsing(t_cmd **new_cmd,
					t_token **token_list, t_redirect_type *redir_type);
int				add_redirects(t_redirect **redir_list, t_redirect_type type,
					t_token **token_list);
int				create_redirect(t_redirect **new_redir, t_token **token_list,
					t_redirect_type type);
t_redirect_type	token_to_redirect_type(t_token_type token_type);
int				parse_redirection(t_cmd **new_cmd, t_token **token_list);
// int				create_redirect(t_redirect **new_redir, char *file,
// 					t_redirect_type type);
int				handle_redirect(t_cmd **new_cmd, t_token **token_list,
					char **args, int count);

/* Abstract Syntax Tree */
t_ast			*create_ast_node(int type);
t_ast			*convert_cmd_to_ast(t_cmd *cmd_list);
void			free_ast(t_ast *node, t_shell *mshell);

/* Working history functions */
void			store_history(char *line, t_token **history_head);
void			print_working_history(t_token *history_head);
void			clear_working_history(t_token **history_head);

/* Helper functions */
int				array_size(char **arr);
void			print_error(char *msg);
int				ft_isspace(char c);
int				ft_isspecial(char c);
int				linked_list_size(t_token *head);

/* Helper functions to free */
void			free_string(char *s);
void			free_array(char **arr, int i);
void			free_token_list(t_token *tokens);
void			free_cmd_list(t_cmd *head);
char			*str_join_result_and_free(char **s1, char *s2);
char			*char_join_result_and_free(char **s1, char c);

/* Heredoc functions */
int				open_heredoc_pipe(t_shell *mshell, t_redirect *redir);
// void			exe_handle_dollar_expansion(char *input, int fd_write,
// 					t_shell *ms);
char			*get_delimiter(char *file);
char			*make_heredoc_filename(int id);
int				process_heredocs(t_shell *mshell, t_ast *node);

/* Execution functions */
void			free_split(char **tab);
int				display_error_cmd(char *cmd);
int				is_white_spaces_cmd(char *cmd);
int				is_ambiguous_redirect(t_shell *mshell, t_redirect *redir);
char			*find_cmd_path(t_shell *mshell, char *cmd);
int				exe_redirection(t_redirect *redir, t_shell *mshell);
int				execute_pipe(t_ast *ast, t_shell *shell);
int				execute_command(t_ast *node, t_shell *mshell);
int				execute_ast(t_ast *node, t_shell *mshell);
int				handle_single_redirection(t_shell *mshell, t_redirect *redir);
int				execute_builtin(t_shell *mshell, char **token);
int				is_builtin(char *cmd);
char			*handle_path_error(t_shell *mshell, char *path, int code);
char			**find_path(char **env);
int				is_fully_quoted(const char *str);
void			safe_close_fds(int in_fd, int out_fd);
int 			redirect_and_backup_fds(t_ast *node, t_shell *mshell, int *in_fd, int *out_fd);
int				wait_command(t_shell *mshell, pid_t pid, int *status,
					int update_exit_code);
int				error_msg(t_shell *mshell, const char *msg,
					int use_errno);

/* Env functions */
void			env_free(t_shell *mshell);
char			**env_dup(char **envp);
char			*env_find_variable(t_shell *mshell, const char *key, size_t *i);
char			*env_find_value(t_shell *mshell, const char *key);
int				env_add(t_shell *mshell, const char *key, const char *value);
int				env_remove(t_shell *mshell, const char *key);
void			env_sort(char **envp, size_t len);
void			env_swap_last(char **envp);
void			env_print(char **envp);
int				env_set_last_argument(t_shell *mshell, char **cmd);
// size_t			strlen_until_char(const char *s, const char ch);
void			free_array_null(char ***array);
char			**realloc_env(char **envp, size_t len);

/* Built-in functions */
int				builtin_cd(t_shell *mshell, char **token);
int				builtin_echo(t_shell *mshell, char **token);
int				builtin_env(t_shell *mshell, char **token);
int				builtin_pwd(t_shell *mshell);
int				builtin_unset(t_shell *mshell, char **token);
void			builtin_exit(t_shell *mshell, char **token);
int				builtin_export(t_shell *mshell, char **token);
void			update_pwd(t_shell *mshell);
int				export_is_valid_key(const char *str);
void			export_print(const char *env);
char			**copy_and_sort_envp(char **envp, size_t *out_count);
int				export_plus_equal(t_shell *mshell, const char *arg, char *plus_equal);
int				export_with_equal(t_shell *mshell, const char *arg, char *equal);

/* Signal functions */
void			setup_signals(t_shell *mshell, int mode);
void			sig_exit_code(t_shell *mshell);
//int				set_signal(t_shell *mshell, void (*sigint_handler)(int), void (*sigquit_handler)(int))

/* To remove block before submiting */
void			print_cmd_list(t_cmd *head);
void			print_redirect_list(t_redirect *redir_list);
void			print_linked_list(t_token *head);
void			print_array(char **arr);

#endif