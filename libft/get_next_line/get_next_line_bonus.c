/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:32:31 by tthajan           #+#    #+#             */
/*   Updated: 2025/05/24 16:39:37 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static t_fd_node *find_or_create_node(t_fd_node **head, int fd)
{
	t_fd_node *current;
	t_fd_node *new_node;

	current = *head;
	while (current)
	{
		if (current->fd == fd)
			return (current);
		current = current->next;
	}
	new_node = malloc(sizeof(t_fd_node));
	if (!new_node)
		return (NULL);
	new_node->fd = fd;
	new_node->backup = NULL;
	new_node->next = *head;
	*head = new_node;
	return (new_node);
}

static void remove_node(t_fd_node **head, int fd)
{
	t_fd_node *current;
	t_fd_node *prev;

	current = *head;
	prev = NULL;
	while (current)
	{
		if (current->fd == fd)
		{
			if (prev)
				prev->next = current->next;
			else
				*head = current->next;
			free(current->backup);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

static char *read_from_fd(int fd, char *buffer, char *backup)
{
	ssize_t bytes_read;
	char *temp;

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
			break;
		buffer[bytes_read] = '\0';
		if (!backup)
			backup = ft_strdup("");
		temp = backup;
		backup = ft_strjoin(temp, buffer);
		free(temp);
		if (ft_strchr(buffer, '\n'))
			break;
	}
	return (backup);
}

static char *extract_line(char **backup)
{
	char *line;
	char *leftover;
	size_t line_len;

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

char *get_next_line(int fd)
{
	static t_fd_node *fd_list;
	t_fd_node *node;
	char *buffer;
	char *line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	node = find_or_create_node(&fd_list, fd);
	if (!node)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	node->backup = read_from_fd(fd, buffer, node->backup);
	free(buffer);
	if (!node->backup)
	{
		remove_node(&fd_list, fd);
		return (NULL);
	}
	line = extract_line(&node->backup);
	if (!line || *line == '\0')
	{
		free(line);
		remove_node(&fd_list, fd);
		return (NULL);
	}
	if (!node->backup)
		remove_node(&fd_list, fd);
	return (line);
}
