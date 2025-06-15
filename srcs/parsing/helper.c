/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:37:07 by trpham            #+#    #+#             */
/*   Updated: 2025/06/15 10:31:01 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	array_size(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		i++;
	}
	return (i);
}

void	print_error(char *msg)
{
	if (msg)
	{
		ft_putstr_fd("Error: ", 2);
		ft_putstr_fd(msg, 2);
		ft_putstr_fd("\n", 2);
	}
}

int	ft_isspace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (TRUE);
	return (FALSE);
}

int	ft_isspecial(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (TRUE);
	return (FALSE);
}

void	reset_exp(t_expand exp)
{
	free_string(exp.ori_value);
	exp.ori_value = NULL;
	free_string(exp.result);
	exp.result = NULL;
	// exp.i = 0; //need??
	// exp.is_expanded = FALSE; //need?? 
}

// void	int_exp(t_expand exp) //--> cannot make if not hide
// {
// 	exp.i = 0;
// 	exp.is_expanded = FALSE;
// 	exp.line = NULL;
// 	exp.ori_value = NULL;
// 	exp.result = NULL;
// }