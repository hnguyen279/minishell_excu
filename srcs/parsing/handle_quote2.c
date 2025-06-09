/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 15:34:40 by trpham            #+#    #+#             */
/*   Updated: 2025/06/09 13:59:14 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

// int	empty_single_quote_check(char **part, int *i, int start_pos)
// {
// 	if (*i - start_pos == 0)
// 	{
// 		*part = ft_strdup("\'\'");
// 		if (!*part)
// 			return (FALSE);
// 		(*i)++;
// 	}
// 	return (TRUE);
// }

int	substr_and_move_index(char *line, char **part, int *i, int start_pos)
{
	*part = ft_substr(line, start_pos, *i - start_pos);
	if (!*part)
	{
		print_error("Malloc failed to substr");
		return (FALSE);
	}
	(*i)++;
	return (TRUE);
}

// int	empty_double_quote_check(char **part, int *i, int start_pos)
// {
// 	if (*i - start_pos == 0)
// 	{
// 		*part = ft_strdup("\"\"");
// 		if (!*part)
// 			return (FALSE);
// 		(*i)++;
// 	}
// 	return (TRUE);
// }
