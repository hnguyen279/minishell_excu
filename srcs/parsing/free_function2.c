/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_function2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 22:36:56 by trpham            #+#    #+#             */
/*   Updated: 2025/06/13 13:28:24 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	*str_join_result_and_free(char **s1, char *s2)
{
	char	*joined_str;

	joined_str = ft_strjoin(*s1, s2);
	free_string(*s1);
	if (!joined_str)
	{
		print_error("ft_strjoin failed");
		return (NULL);
	}
	return (joined_str);
}

char	*char_join_result_and_free(char **s1, char c)
{
	char	*joined_str;
	char	*s2;

	s2 = ft_calloc(2, 1);
	s2[0] = c;
	joined_str = str_join_result_and_free(s1, s2);
	free_string(s2);
	return (joined_str);
}
