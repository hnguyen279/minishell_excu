/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_remove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:04:56 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/08 16:04:58 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	env_remove(t_shell *mshell, const char *key)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!mshell || !env_find_variable(mshell, key, &i))
		return (0);
	free(mshell->envp[i]);
	j = i + 1;
	while (mshell->envp[j])
	{
		mshell->envp[i] = mshell->envp[j];
		i++;
		j++;
	}
	mshell->envp[i] = NULL;
	return (1);
}
