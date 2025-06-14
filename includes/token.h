/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:05:38 by trpham            #+#    #+#             */
/*   Updated: 2025/06/14 15:50:11 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

typedef struct s_shell	t_shell;
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

typedef struct s_token
{
	char				*value;
	char				*ori_value;
	t_token_type		type;
	struct s_token		*next;
	struct s_token		*prev;
}	t_token;

typedef struct s_expand
{
	char	*line;
	char	*ori_value;
	char	*result;
	int		is_expanded;
	int		i;
	
}	t_expand;


/* Tokenization */
// int				tokenization_expansion_validation(char *line, t_shell *mshell,
// 					t_token **token_list);
// int	init_and_validate_input(char *line, t_shell *mshell);
int	tokenization_expansion_validation(char *line, t_shell *mshell);
int				skip_expanded_empty_var(t_token **token_list);
void			skip_middle_empty_vars(t_token **token_list);
void			skip_first_empty_vars(t_token **token_list);
t_token			*create_token(char *s, char *ori_s, t_token_type i);
int				handle_pipe(t_token **token_list, int *i);
int				handle_in_heredoc(char *line, t_token **token_list, int *i);
int				handle_out_append(char *line, t_token **token_list, int *i);
void			add_token(t_token **tokenized_input_list, t_token *new_token);
t_token			*convert_user_input_to_token(char *line, t_shell *mshell);
int				handle_word(char *line, t_token **token_list, int *i,
					t_shell *mshell);
// int	reset_exp(t_expand exp);
char			*extract_ori_word(char *line, int *i);
char			*extract_full_word(char *line, int *i, t_shell *mshell);
char			*handle_single_quote(char *line, int *i);
char			*handle_double_quote(char *line, int *i, t_shell *mshell);
char			*extract_unquoted_word(char *line, int *i, t_shell *mshell);
int				substr_and_move_index(char *line, char **part, int *i,
					int start_pos);
void	int_exp(t_expand exp);
void	reset_exp(t_expand exp);
/* Expansion token */
char			*expand_token_value(char *str, t_shell	*mshell);
void			handle_dollar_sign(char *str, t_shell *mshell, char **result,
					int *i);
char			*handle_env_variable(char **str, t_shell *mshell, int *i,
					char *result);
char			*expand_exit_code(t_shell *mshell, char	*result, int *i);
char			*extract_variable_name(char **str, char **var_name, int *i,
					char **result);

/* Retokenization */
int				retokenizer(t_token **token_list);
t_token			*replace_token_with_new_arr(t_token *current, char **arr);
int				link_split_token(t_token **current, t_token **prev_token,
					t_token **next_token, t_token **token_list);
int				check_split_token_condition(t_token **current, t_token **prev_token);
int				handle_split_token(t_token **current, t_token **prev_token,
					t_token **next_token, t_token **token_list);

					

#endif