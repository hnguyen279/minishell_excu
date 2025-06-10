/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:30:20 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/10 17:02:32 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	print_echo_from(char **token, int i)
{
	while (token[i])
	{
		write(STDOUT_FILENO, token[i], ft_strlen(token[i]));
		if (token[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
}

int	builtin_echo(t_shell *mshell, char **token)
{
	int	i;
	int	newline;
	int	j;

	i = 1;
	newline = 1;
	// printf("debug in echo \n");
	// print_array(token);//debug
	while (token[i] && token[i][0] == '-' && token[i][1] == 'n')
	{
		j = 2;
		while (token[i][j] == 'n')
			j++;
		if (token[i][j] != '\0')
			break ;
		newline = 0;
		i++;
	}
	print_echo_from(token, i);
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	mshell->exit_code = 0;
	return (0);
}
