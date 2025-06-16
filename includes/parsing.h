/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:15:54 by trpham            #+#    #+#             */
/*   Updated: 2025/06/16 15:16:14 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

typedef struct s_token t_token;
typedef enum e_token_type t_token_type;
typedef struct s_cmd t_cmd;
typedef	enum e_redirect_type t_redirect_type;
typedef struct s_redirect t_redirect;

typedef enum e_node_type
{
	NODE_NONE = 0,
	NODE_CMD,
	NODE_PIPE
}	t_node_type;

typedef struct s_cmd
{
	char				*cmd_name;
	char				**args;
	t_redirect			*redirects;
	struct s_cmd		*next;
}	t_cmd;

typedef enum e_redirect_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	REDIR_INVALID
}	t_redirect_type;

typedef struct s_ast
{
	t_node_type			type;
	char				**cmd;
	t_redirect			*redirects;
	struct s_ast		*left;
	struct s_ast		*right;
}	t_ast;

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
int				check_redir_type_before_parsing(t_cmd **new_cmd,
					t_token **token_list, t_redirect_type *redir_type);
int				add_redirects(t_redirect **redir_list, t_redirect_type type,
					t_token **token_list);
int				create_redirect(t_redirect **new_redir, t_token **token_list,
					t_redirect_type type);
t_redirect_type	token_to_redirect_type(t_token_type token_type);
int				parse_redirection(t_cmd **new_cmd, t_token **token_list);
int				handle_redirect(t_cmd **new_cmd, t_token **token_list,
					char **args, int count);
int	check_ambiguous_redirect(t_token **token_list);


/* Abstract Syntax Tree */
t_ast			*create_ast_node(int type);
t_ast			*convert_cmd_to_ast(t_cmd *cmd_list);
//void			free_ast(t_ast *node, t_shell *mshell);
void			free_ast(t_ast *node);

#endif