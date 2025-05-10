/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 17:41:02 by thi-huon          #+#    #+#             */
/*   Updated: 2024/11/20 17:41:06 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

static	char	*extract_line(char *s)
{
	int		i;
	char	*line;

	i = 0;
	while (s[i] != '\0' && s[i] != '\n')
		i++;
	if (s[i] == '\n')
		line = ft_substr_gnl(s, 0, (i + 1));
	else
		line = ft_strdup_gnl(s);
	return (line);
}

static	char	*remain_buffer(char *s)
{
	int		i;
	char	*remain_lines;

	i = 0;
	while (s[i] != '\0' && s[i] != '\n')
		i++;
	if (s[i] == '\n')
		remain_lines = ft_substr_gnl(s, i + 1, (ft_strlen_gnl(s) - i - 1));
	else
		remain_lines = NULL;
	free(s);
	return (remain_lines);
}

static	char	*read_update_buffer(int fd, char *buffer, char *read_buf)
{
	ssize_t	bytes_read;
	char	*temp;

	bytes_read = read(fd, read_buf, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		read_buf[bytes_read] = '\0';
		if (!buffer)
			buffer = ft_strdup_gnl(read_buf);
		else
		{
			temp = ft_strjoin_gnl(buffer, read_buf);
			free(buffer);
			buffer = temp;
		}
		if (ft_strchar_gnl(buffer, '\n', ft_strlen_gnl(buffer)))
			break ;
		bytes_read = read(fd, read_buf, BUFFER_SIZE);
	}
	free(read_buf);
	if (bytes_read < 0 && buffer)
		return (free(buffer), NULL);
	else if (bytes_read < 0 && !buffer)
		return (NULL);
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*read_buf;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	read_buf = malloc(BUFFER_SIZE + 1);
	if (!read_buf)
		return (NULL);
	buffer = read_update_buffer(fd, buffer, read_buf);
	if (!buffer)
		return (NULL);
	if (buffer[0] == '\0')
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	line = extract_line(buffer);
	buffer = remain_buffer(buffer);
	return (line);
}
