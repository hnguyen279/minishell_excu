/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_ambiguous.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 20:37:45 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/18 20:37:45 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int	is_multiple_words(const char *str)
{
	int	i;
	int	in_word;
	int	word_count;

	i = 0;
	in_word = 0;
	word_count = 0;
	while (str[i])
	{
		if (str[i] != ' ' && (str[i] < 9 || str[i] > 13))
		{
			if (!in_word)
			{
				in_word = 1;
				word_count++;
			}
		}
		else
			in_word = 0;
		i++;
	}
	if (word_count > 1)
		return (1);
	else
		return (0);
}

static int	ambiguous_redirect_error(t_shell *mshell, const char *ori_file)
{
	ft_printf_fd(2, "minishell: %s: ambiguous redirect\n", ori_file);
	mshell->exit_code = 1;
	return (1);
}

int	is_ambiguous_redirect(t_shell *mshell, t_redirect *redir)
{
	if (!redir || !redir->file || !redir->ori_file)
		return (0);
	if (redir->type == REDIR_HEREDOC)
		return (0);
	if (redir->file[0] == '\0' && redir->ori_file[0] != '\0'
		&& ft_strchr(redir->ori_file, '$') && !is_fully_quoted(redir->ori_file))
		return (ambiguous_redirect_error(mshell, redir->ori_file));
	if (redir->file[0] != '\0' && redir->ori_file[0] != '\0'
		&& ft_strchr(redir->ori_file, '$') && !is_fully_quoted(redir->ori_file)
		&& is_white_spaces_cmd(redir->file))
		return (ambiguous_redirect_error(mshell, redir->ori_file));
	if (is_multiple_words(redir->file) && ft_strchr(redir->ori_file, '$')
		&& !is_fully_quoted(redir->ori_file)
		&& !is_white_spaces_cmd(redir->file))
		return (ambiguous_redirect_error(mshell, redir->ori_file));
	return (0);
}
