/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 09:15:43 by tthajan           #+#    #+#             */
/*   Updated: 2025/05/24 11:21:03 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_from_fd(int fd, char *buffer, char *backup)
{
	ssize_t	bytes_read;
	char	*temp;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(backup);
			return (NULL);
		}
		else if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		if (!backup)
			backup = ft_strdup("");
		temp = backup;
		backup = ft_strjoin(temp, buffer);
		free(temp);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (backup);
}

static char	*extract_line(char **backup)
{
	char	*line;
	char	*leftover;
	size_t	line_len;

	line_len = 0;
	while ((*backup)[line_len] && (*backup)[line_len] != '\n')
		line_len++;
	if ((*backup)[line_len] == '\n')
	{
		line = ft_substr(*backup, 0, line_len + 1);
		leftover = ft_strdup(*backup + line_len + 1);
	}
	else
	{
		line = ft_strdup(*backup);
		leftover = NULL;
	}
	free(*backup);
	*backup = leftover;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*backup;
	char		*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	backup = read_from_fd(fd, buffer, backup);
	free(buffer);
	buffer = NULL;
	if (!backup)
		return (NULL);
	line = extract_line(&backup);
	if (!line || *line == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}
