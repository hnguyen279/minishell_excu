/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_last_argument.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:04:47 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/18 20:47:55 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	env_set_last_argument(t_shell *mshell, char **cmd)
{
	size_t	i;
	char	*last_arg;

	if (!mshell || !cmd || !cmd[0])
		return (1);
	i = 0;
	while (cmd[i])
		i++;
	if (i == 0)
		return (0);
	last_arg = cmd[i - 1];
	if (!last_arg[0] || (ft_strcmp(last_arg, "<") == 0)
		|| (ft_strcmp(last_arg, ">") == 0) || (ft_strcmp(last_arg, "<<") == 0)
		|| (ft_strcmp(last_arg, ">>") == 0)
		|| (mshell->token_list->type == PIPE))
	{
		return (0);
	}
	return (env_add(mshell, "_", last_arg));
}
