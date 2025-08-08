/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:57:08 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/08 15:45:59 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include <errno.h>

static int	setup_heredoc_pipe(int *pipefd)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

static void	process_heredoc_input(char *delimiter, int write_fd)
{
	char	*line;
	char	*expanded_line;

	write(STDOUT_FILENO, "heredoc> ", 9);
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& line[ft_strlen(delimiter)] == '\n')
		{
			free(line);
			break ;
		}
		expanded_line = expand_env_vars(line);
		if (expanded_line)
		{
			write(write_fd, expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		else
			write(write_fd, line, ft_strlen(line));
		free(line);
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
	}
}

int	handle_heredoc(char *delimiter)
{
	int	pipefd[2];

	if (setup_heredoc_pipe(pipefd) == -1)
		return (-1);
	process_heredoc_input(delimiter, pipefd[1]);
	close(pipefd[1]);
	return (pipefd[0]);
}
