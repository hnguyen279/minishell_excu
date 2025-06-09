/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:22:38 by trpham            #+#    #+#             */
/*   Updated: 2025/06/09 13:59:29 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	*extract_full_word(char *line, int *i, t_shell *mshell)
{
	char	*result;
	char	*part;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (line[*i] && ft_isspace(line[*i]) && ft_isspecial(line[*i]))
	{
		if (line[*i] == '$' && (line[*i + 1] == '\'' || line[*i + 1] == '"'))
			(*i)++;
		if (line[*i] == '\'')
			part = handle_single_quote(line, i);
		else if (line[*i] == '"')
		{
			// printf("debug 1\n"); //debug
			part = handle_double_quote(line, i, mshell);
		}
		else
			part = extract_unquoted_word(line, i, mshell);
		if (!part)
		{
			free(result);
			return (NULL);
		}
		result = str_join_result_and_free(&result, part);
		free(part);
	}
	return (result);
}

char	*handle_single_quote(char *line, int *i)
{
	char	*part;
	int		start_pos;

	part = NULL;
	(*i)++;
	start_pos = *i;
	while (line[*i] && line[*i] != '\'')
		(*i)++;
	if (line[*i] != '\'')
	{
		print_error("Unclosed quote");
		return (NULL);
	}
	// if (empty_single_quote_check(&part, i, start_pos) == TRUE)
	// 	return (part);
	// else if (empty_single_quote_check(&part, i, start_pos) == FALSE)
	// 	return (NULL);
	if (substr_and_move_index(line, &part, i, start_pos) == FALSE)
		return (NULL);
	return (part);
}

char	*handle_double_quote(char *line, int *i, t_shell *mshell)
{
	char	*part;
	int		start_pos;
	char	*tmp;

	part = NULL;
	(*i)++;
	start_pos = *i;
	while (line[*i] && line[*i] != '\"')
		(*i)++;
	if (line[*i] != '\"')
	{
		print_error("Unclosed quote");
		return (NULL);
	}
	// if (*i - start_pos == 0)
	// {
	// 	*part = ft_strdup("\"\"");
	// 	if (!*part)
	// 		return (NULL);
	// 	(*i)++;
	// }
	// if (empty_double_quote_check(&part, i, start_pos) == TRUE)
	// 	return (part);
	// else if (empty_double_quote_check(&part, i, start_pos) == FALSE)
	// 	return (NULL);
	if (substr_and_move_index(line, &part, i, start_pos) == FALSE)
		return (NULL);
	// printf("part %s\n", part);
	// printf("debug 2\n"); //debug
	tmp = part;
	part = expand_token_value(part, mshell);
	free_string(tmp);
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
			|| ft_isspecial(line[*i]) == TRUE)
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
