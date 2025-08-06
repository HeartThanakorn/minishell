/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:00:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 17:12:48 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child_process(t_cmd *current, int input_fd, int *pipefd)
{
	setup_child_signals();
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (current->next && current->next->cmd)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	exec_single_cmd(current, STDIN_FILENO, STDOUT_FILENO);
	exit(0);
}

void	handle_parent_process(int *input_fd, int *pipefd, t_cmd *current)
{
	if (*input_fd != STDIN_FILENO)
		close(*input_fd);
	if (current->next && current->next->cmd)
	{
		close(pipefd[1]);
		*input_fd = pipefd[0];
	}
}

void	wait_for_children(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				set_last_exit_status(WEXITSTATUS(status));
			else
				set_last_exit_status(1);
		}
		i++;
	}
}
