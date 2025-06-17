/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 11:47:59 by trpham            #+#    #+#             */
/*   Updated: 2025/06/17 13:13:35 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_token	*convert_user_input_to_token(char *line, t_shell *mshell)
{
	int		i;
	t_token	*token_list;
	int		is_token_list;

	token_list = NULL;
	is_token_list = TRUE;
	i = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]) == TRUE)
			i++;
		else
			is_token_list = handle_token_type(line, &token_list, &i, mshell);
		if (is_token_list == FALSE)
			return (NULL);
	}
	if (retokenizer(&token_list) == FALSE)
	{
		free_token_list(token_list);
		return (NULL);
	}
	return (token_list);
}

int	handle_token_type(char *line, t_token **token_list, int *i, t_shell *mshell)
{
	int	is_token_list;

	if (line[*i] == '|')
		is_token_list = handle_pipe(token_list, i);
	else if (line[*i] == '<')
		is_token_list = handle_in_heredoc(line, token_list, i);
	else if (line[*i] == '>')
		is_token_list = handle_out_append(line, token_list, i);
	else
		is_token_list = handle_word(line, token_list, i, mshell);
	return (is_token_list);
}

int	handle_pipe(t_token **token_list, int *i)
{
	t_token	*new_token;

	new_token = NULL;
	new_token = create_token("|", "|", PIPE);
	if (!new_token)
		return (FALSE);
	add_token(token_list, new_token);
	(*i)++;
	return (TRUE);
}

int	handle_in_heredoc(char *line, t_token **token_list, int *i)
{
	t_token	*new_token;

	new_token = NULL;
	if (line[*i + 1] == '<')
	{
		new_token = create_token("<<", "<<", REDIR_HEREDOC_TOKEN);
		*i += 2;
	}
	else
	{
		new_token = create_token("<", "<", REDIR_IN_TOKEN);
		(*i)++;
	}
	if (!new_token)
		return (FALSE);
	add_token(token_list, new_token);
	return (TRUE);
}

int	handle_out_append(char *line, t_token **token_list, int *i)
{
	t_token	*new_token;

	new_token = NULL;
	if (line[*i + 1] == '>')
	{
		new_token = create_token(">>", ">>", REDIR_APPEND_TOKEN);
		*i += 2;
	}
	else
	{
		new_token = create_token(">", ">", REDIR_OUT_TOKEN);
		(*i)++;
	}
	if (!new_token)
		return (FALSE);
	add_token(token_list, new_token);
	return (TRUE);
}
