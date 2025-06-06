/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:22:38 by trpham            #+#    #+#             */
/*   Updated: 2025/06/06 16:07:25 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char *extract_full_word(char *line, int *i, t_shell *mshell)
{
	char	*result;
	char	*part;
	// char	*tmp;

	result = ft_strdup("");
	if (!result)
	{
		print_error("malloc error\n");
		return (NULL);
	}
	while (line[*i] && ft_isspace(line[*i]) == FALSE &&
		   line[*i] != '<' && line[*i] != '>' && line[*i] != '|')
	{
		if (line[*i] == '$' && (line[*i + 1] == '\'' || line[*i + 1] == '"'))
			(*i)++;
		if (line[*i] == '\'')
			part = handle_single_quote(line, i);
		else if (line[*i] == '"')
			part = handle_double_quote(line, i, mshell);
		else
			part = extract_unquoted_word(line, i, mshell);
		if (!part)
		{
			free(result);
			return (NULL);
		}
		result = str_join_result_and_free(&result, part);
		// tmp = result;
		// result = ft_strjoin(result, part);
		// free(tmp);
		free(part);
	}
	return result;
}

char	*handle_single_quote(char *line, int *i)
{
	char	*part;
	int		start_pos;
	
	part = NULL;
	// *in_single_quote = TRUE;
	(*i)++;
	start_pos = *i;
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	if (line[*i] != '\'')
	{
		print_error("Unclosed quote");
		return (NULL);
	}
	part = ft_substr(line, start_pos, *i - start_pos);
	if (!part)
	{
		print_error("Malloc failed to substr");
		return (NULL);
	}
	(*i)++;
	// *in_single_quote = FALSE;
	return (part);
}

	char	*handle_double_quote(char *line, int *i, t_shell *mshell)
{
	char	*part;
	int		start_pos;
	char	*tmp;
	
	part = NULL;
	// *in_double_quote = TRUE;
	(*i)++;
	start_pos = *i;
	while (line[*i] && line[*i] != '\"')
		(*i)++;
	if (line[*i] != '\"')
	{
		print_error("Unclosed quote");
		return (NULL);
	}
	part = ft_substr(line, start_pos, *i - start_pos);
	if (!part)
	{
		print_error("Malloc failed to substr");
		return (NULL);
	}
	(*i)++;
	tmp = part;
	part = expand_token_value(part, mshell);
	free_string(tmp);
	// *in_double_quote = FALSE;
	return (part);
}

char	*extract_unquoted_word(char *line, int *i, t_shell *mshell)
{
	int		start_pos;
	char	*part;
	char	*tmp;

	start_pos = *i;
	while (line[*i])
	{
		if (ft_isspace(line[*i]) == TRUE || line[*i] == '\'' || line[*i] == '\"'
			|| line[*i] == '<' || line[*i] == '>' || line[*i] == '|')
			break ;
		(*i)++;
	}
	part = ft_substr(line, start_pos, *i - start_pos);
	if (!part)
		return (NULL);
	tmp = part;
	part = expand_token_value(part, mshell);
	free_string(tmp);
	return (part);
}
