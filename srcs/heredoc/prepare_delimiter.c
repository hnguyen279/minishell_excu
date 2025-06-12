/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_delimiter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:05:46 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/12 20:17:33 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	process_quote_and_copy(const char *word, char *res, int *i, int *j)
{
	char	quote;

	quote = word[*i];
	(*i)++;
	while (word[*i] && word[*i] != quote)
		res[(*j)++] = word[(*i)++];
	if (word[*i] == quote)
		(*i)++;
}

static char	*get_delimiter(char *word)
{
	int		i;
	int		j;
	char	*res;

	if (!word)
		return (NULL);
	res = malloc(ft_strlen(word) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (word[i])
	{
		if (word[i] == '\'' || word[i] == '"')
			process_quote_and_copy(word, res, &i, &j);
		else
			res[j++] = word[i++];
	}
	res[j] = '\0';
	return (res);
}

int	prepare_delimiter(t_redirect *redir, char **delim, int *expand)
{
	int	quote_type;

	quote_type = is_fully_quoted(redir->ori_file);
	if (quote_type == -1)
	{
		ft_printf_fd(2,
			"minishell: heredoc: unmatched quote in delimiter: %s\n",
			redir->ori_file);
		return (1);
	}
	if (quote_type == 1 || quote_type == 2)
		*expand = 0;
	else
		*expand = 1;
	*delim = get_delimiter(redir->ori_file);
	if (!*delim)
	{
		ft_printf_fd(2, "minishell: heredoc: memory allocation failed\n");
		return (1);
	}
	return (0);
}
