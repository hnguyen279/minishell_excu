/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 21:44:47 by thi-huon          #+#    #+#             */
/*   Updated: 2024/11/05 21:44:52 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total_size;
	char	*dest;
	size_t	i;

	total_size = nmemb * size;
	if (nmemb != 0 && (total_size / nmemb) != size)
		return (NULL);
	dest = (char *)malloc(total_size);
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (i < total_size)
	{
		dest[i] = '\0';
		i++;
	}
	return ((void *)dest);
}
