/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_support_exe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:06:48 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/12 20:41:35 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	is_white_spaces_cmd(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] != ' ' && (cmd[i] < 9 || cmd[i] > 13))
			return (0);
		i++;
	}
	return (1);
}

int	error_msg(t_shell *mshell, const char *msg, int use_errno)
{
	if (use_errno)
		ft_printf_fd(2, "minishell: %s: %s\n", msg, strerror(errno));
	else
		ft_printf_fd(2, "minishell: %s\n", msg);
	if (mshell)
		mshell->exit_code = 1;
	return (1);
}

char	*handle_path_error(t_shell *mshell, char *path, int code)
{
	if (code == 126)
		ft_printf_fd(STDERR_FILENO, "minishell: %s: Permission denied\n", path);
	else if (code == 127)
		ft_printf_fd(STDERR_FILENO, "minishell: %s: command not found\n", path);
	mshell->exit_code = code;
	return (NULL);
}

static int	check_intermediate_quote(const char *str, size_t start, size_t end,
		char quote)
{
	size_t	i;

	i = start;
	while (i < end)
	{
		if (str[i] == quote)
			return (2);
		i++;
	}
	return (-1);
}

int	is_fully_quoted(const char *str)
{
	size_t	i;
	size_t	len;
	char	quote;

	i = 0;
	if (!str || !*str)
		return (0);
	len = strlen(str);
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] != '\'' && str[i] != '"')
		return (0);
	quote = str[i++];
	while (len > i && ((str[len - 1] == ' ')
			|| (str[len - 1] >= 9 && str[len - 1] <= 13)))
		len--;
	if (str[len - 1] != quote)
		return (check_intermediate_quote(str, i, len - 1, quote));
	return (1);
}
