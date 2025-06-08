/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:31:51 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/08 15:31:53 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	builtin_unset(t_shell *mshell, char **token)
{
	size_t	i;

	if (!mshell || !token || !token[0])
		return (error_msg(mshell, "unset: internal error", 0));
	i = 1;
	while (token[i])
	{
		if (token[i][0] == '-')
		{
			ft_printf_fd(2, "minishell: unset: %s: invalid option\n", token[i]);
			mshell->exit_code = 2;
			return (2);
		}
		env_remove(mshell, token[i]);
		i++;
	}
	mshell->exit_code = 0;
	return (0);
}
