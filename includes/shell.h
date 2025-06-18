/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 06:09:47 by trpham            #+#    #+#             */
/*   Updated: 2025/06/19 02:41:38 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "../libft/includes/libft.h"
# include "parsing.h"
# include "token.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

# define TRUE 0
# define FALSE -1
# define FD_READ 0
# define FD_WRITE 1
# define MODE_INTERACTIVE 0
# define MODE_HEREDOC 1
# define MODE_CHILD 2

extern volatile sig_atomic_t	g_signum;
typedef struct s_redirect
{
	int							fd;
	t_redirect_type				type;
	char						*file;
	char						*ori_file;
	char						*tmp_file;
	struct s_redirect			*next;
}								t_redirect;
typedef struct s_shell
{
	int							exit_code;
	char						**envp;
	int							heredoc_index;
	int							has_pipe;
	t_ast						*tree;
	t_token						*token_list;
	t_cmd						*cmd_list;
	t_token						*history_head;
	char						*curr_pwd;
	char						*old_pwd;
}								t_shell;

/* Shell initialization */
int		init_shell(t_shell *mshell, char **envp);
void	shell_interactive(t_shell *mshell);
void	shell_cleanup(t_shell *mshell);
void	loop_clean(t_shell *mshell);

/* Handle user input */
char	*read_user_input(t_shell *mshell);
int		process_non_empty_input(char *line, t_shell *mshell);
int		process_user_line(char *line, t_shell *mshell);
int		init_and_validate_input(char *line, t_shell *mshell);
void	process_valid_line(t_shell *mshell);
void	run_ast_pipeline(t_shell *mshell);
void	cleanup_heredoc_tempfiles(t_ast *tree);

/* Validate input */
int		validate_token(t_token *token);
int		is_pipe(t_token *token);
int		is_valid_pipe(t_token *token);
int		is_redirection(t_token *token);
int		is_valid_redirection(t_token *token_list);
int		validate_quote(char *line);
int		is_operator(t_token *token);

/* Working history functions */
int		store_history(char *line, t_token **history_head);
void	print_working_history(t_token *history_head);
void	clear_working_history(t_token **history_head);

/* Helper functions */
int		array_size(char **arr);
void	print_error(char *msg);
int		ft_isspace(char c);
int		ft_isspecial(char c);
int		linked_list_size(t_token *head);

/* Helper functions to free */
void	free_string(char *s);
void	free_token(t_token *token);
void	free_token_list(t_token *tokens);
void	free_cmd_list(t_cmd *head);
char	*str_join_result_and_free(char **s1, char *s2);
char	*char_join_result_and_free(char **s1, char c);

/* Heredoc functions */
int		create_tmp_file(t_shell *mshell, t_redirect *redir, char **path,
			int *fd);
int		cleanup_heredoc_failure(t_redirect *redir, int fd, const char *path);
int		write_heredoc(t_shell *mshell, int fd, const char *delim, int expand);
int		prepare_delimiter(t_redirect *redir, char **delim, int *expand);
int		process_heredocs(t_shell *mshell, t_ast *node);

/* Execution functions */
void	free_split(char **tab);
int		display_error_cmd(char *cmd);
int		is_white_spaces_cmd(char *cmd);
int		is_ambiguous_redirect(t_shell *mshell, t_redirect *redir);
char	*find_cmd_path(t_shell *mshell, char *cmd);
int		exe_redirection(t_redirect *redir, t_shell *mshell);
int		execute_pipe(t_ast *node, t_shell *mshell);
int		execute_command(t_ast *node, t_shell *mshell);
void	execute_ast(t_ast *node, t_shell *mshell);
int		handle_single_redirection(t_shell *mshell, t_redirect *redir);
int		execute_builtin(t_shell *mshell, char **token);
int		is_builtin(char *cmd);
char	*handle_path_error(t_shell *mshell, char *path, int code,
			int check_redir);
char	**find_path(char **env);
int		is_fully_quoted(const char *str);
void	safe_close_fds(int in_fd, int out_fd);
int		redirect_and_backup_fds(t_ast *node, t_shell *mshell, int *in_fd,
			int *out_fd);
int		wait_command(t_shell *mshell, pid_t pid, int *status,
			int update_exit_code);
int		error_msg(t_shell *mshell, const char *msg, int use_errno);

/* Env functions */
int		init_shlvl_env(t_shell *mshell);
int		init_pwd_env(t_shell *mshell);
void	env_free(t_shell *mshell);
char	**env_dup(char **envp);
char	*env_find_variable(t_shell *mshell, const char *key, size_t *i);
char	*env_find_value(t_shell *mshell, const char *key);
int		env_add(t_shell *mshell, const char *key, const char *value);
int		env_remove(t_shell *mshell, const char *key);
void	env_sort(char **envp, size_t len);
void	env_swap_last(char **envp);
void	env_print(char **envp);
int		env_set_last_argument(t_shell *mshell, char **cmd);
void	free_array_null(char ***array);
char	**realloc_env(char **envp, size_t len);

/* Built-in functions */
int		builtin_cd(t_shell *mshell, char **token);
int		builtin_echo(t_shell *mshell, char **token);
int		builtin_env(t_shell *mshell, char **token);
int		builtin_pwd(t_shell *mshell);
int		builtin_unset(t_shell *mshell, char **token);
void	builtin_exit(t_shell *mshell, char **token);
int		builtin_export(t_shell *mshell, char **token);
void	update_pwd(t_shell *mshell);
int		export_is_valid_key(const char *str);
void	export_print(const char *env);
char	**copy_and_sort_envp(char **envp, size_t *out_count);
int		export_plus_equal(t_shell *mshell, const char *arg, char *plus_equal);
int		export_with_equal(t_shell *mshell, const char *arg, char *equal);
int		handle_plus_equal_case(t_shell *mshell, const char *arg,
			char *plus_equal);
int		handle_equal_case(t_shell *mshell, const char *arg, char *equal);

/* Signal functions */
void	setup_signals(t_shell *mshell, int mode);
void	sig_exit_code(t_shell *mshell);
int		child_default_signals(void);

/* To remove block before submiting */
void	print_cmd_list(t_cmd *head);
void	print_redirect_list(t_redirect *redir_list);
void	print_linked_list(t_token *head);
void	print_array(char **arr);

#endif