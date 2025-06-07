/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 11:47:59 by trpham            #+#    #+#             */
/*   Updated: 2025/06/07 11:49:19 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_token	*convert_user_input_to_token(char *line, t_shell *mshell)
{
	int		i;
	t_token	*token_list;

	token_list = NULL;
	i = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]) == TRUE)
			i++;
		else if (line[i] == '|')
			handle_pipe(&token_list, &i);
		else if (line[i] == '<')
			handle_in_heredoc(line, &token_list, &i);
		else if (line[i] == '>')
			handle_out_append(line, &token_list, &i);
		else
		{
			if (handle_word(line, &token_list, &i, mshell) == FALSE)
				return (NULL);
		}
	}
	return (token_list);
}

void	handle_pipe(t_token **token_list, int *i)
{
	t_token	*new_token;

	new_token = NULL;
	new_token = create_token("|", PIPE);
	add_token(token_list, new_token);
	(*i)++;
}

void	handle_in_heredoc(char *line, t_token **token_list, int *i)
{
	t_token	*new_token;

	new_token = NULL;
	if (line[*i + 1] == '<')
	{
		new_token = create_token("<<", REDIR_HEREDOC_TOKEN);
		*i += 2;
	}
	else
	{
		new_token = create_token("<", REDIR_IN_TOKEN);
		(*i)++;
	}
	add_token(token_list, new_token);
}

void	handle_out_append(char *line, t_token **token_list, int *i)
{
	t_token	*new_token;

	new_token = NULL;
	if (line[*i + 1] == '>')
	{
		new_token = create_token(">>", REDIR_APPEND_TOKEN);
		*i += 2;
	}
	else
	{
		new_token = create_token(">", REDIR_OUT_TOKEN);
		(*i)++;
	}
	add_token(token_list, new_token);
}

int	handle_word(char *line, t_token **token_list, int *i, t_shell *mshell)
{
	char	*extracted_str;
	t_token	*new_token;

	extracted_str = extract_full_word(line, i, mshell);
	if (!extracted_str)
	{
		print_error("Can't extract word");
		return (FALSE);
	}
	new_token = create_token(extracted_str, WORD);
	add_token(token_list, new_token);
	free_string(extracted_str);
	return (TRUE);
}
