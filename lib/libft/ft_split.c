/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:47:27 by thi-huon          #+#    #+#             */
/*   Updated: 2024/11/06 12:47:32 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

static int	ft_count(char const *s, char c)
{
	int	i;
	int	count;
	int	word_count;

	i = 0;
	count = 0;
	word_count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && count == 0)
		{
			word_count++;
			count = 1;
		}
		else if (s[i] == c)
		{
			count = 0;
		}
		i++;
	}
	return (word_count);
}

static void	ft_extract_words(char const *s, char c, char **dest)
{
	int	i;
	int	j;
	int	start;
	int	end;

	i = 0;
	j = 0;
	start = -1;
	while (s[i] != '\0')
	{
		if (s[i] != c && start == -1)
			start = i;
		if ((s[i] == c || s[i + 1] == '\0') && start != -1)
		{
			if (s[i] == c)
				end = i;
			else
				end = i + 1;
			dest[j] = ft_substr(s, start, end - start);
			j++;
			start = -1;
		}
		i++;
	}
	dest[j] = NULL;
}

char	**ft_split(char const *s, char c)
{
	int		word_count;
	char	**dest;

	if (s == NULL)
		return (NULL);
	word_count = ft_count(s, c);
	dest = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (dest == NULL)
		return (NULL);
	if (word_count == 0)
	{
		dest[0] = NULL;
		return (dest);
	}
	ft_extract_words(s, c, dest);
	return (dest);
}
