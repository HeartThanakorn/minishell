/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:45:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 17:12:48 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_commands(t_cmd *cmd_list)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		if (current->cmd)
			count++;
		current = current->next;
	}
	return (count);
}

static int	setup_pipeline(t_cmd *cmd_list, pid_t **pids)
{
	int	cmd_count;

	cmd_count = count_commands(cmd_list);
	*pids = malloc(sizeof(pid_t) * cmd_count);
	if (!*pids)
		return (-1);
	return (cmd_count);
}

static int	process_command(t_cmd *current, int *input_fd, pid_t *pids, int i)
{
	int		pipefd[2];
	pid_t	pid;

	if (current->next && current->next->cmd)
	{
		if (create_pipe(pipefd) == -1)
			return (-1);
	}
	pid = fork();
	if (pid == 0)
		handle_child_process(current, *input_fd, pipefd);
	else if (pid > 0)
	{
		pids[i] = pid;
		handle_parent_process(input_fd, pipefd, current);
		return (0);
	}
	else
	{
		perror("fork");
		return (-1);
	}
	return (0);
}

static void	execute_pipeline_loop(t_cmd *cmd_list, pid_t *pids, int *input_fd)
{
	t_cmd	*current;
	int		i;

	current = cmd_list;
	i = 0;
	while (current)
	{
		if (!current->cmd)
		{
			current = current->next;
			continue ;
		}
		if (process_command(current, input_fd, pids, i) == -1)
		{
			free(pids);
			return ;
		}
		current = current->next;
		i++;
	}
}

void	exec_pipeline(t_cmd *cmd_list)
{
	int		input_fd;
	pid_t	*pids;
	int		cmd_count;

	cmd_count = setup_pipeline(cmd_list, &pids);
	if (cmd_count == -1)
		return ;
	input_fd = STDIN_FILENO;
	execute_pipeline_loop(cmd_list, pids, &input_fd);
	wait_for_children(pids, cmd_count);
	free(pids);
}
