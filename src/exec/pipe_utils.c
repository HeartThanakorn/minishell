/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:45:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/08 17:01:21 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_cmd *cmd, char **envp)
{
	if (is_builtin(cmd->cmd))
		exit(execute_builtin(cmd->args));
	if (!cmd->path)
	{
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (execve(cmd->path, cmd->args, envp) == -1)
	{
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
}

void	handle_pipe_signal_exit(int status)
{
	if (WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		set_last_exit_status(130);
	}
	else if (WTERMSIG(status) == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		set_last_exit_status(131);
	}
	else
		set_last_exit_status(128 + WTERMSIG(status));
}

void	handle_signal_exit(int status)
{
	if (WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		set_last_exit_status(130);
	}
	else if (WTERMSIG(status) == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		set_last_exit_status(131);
	}
	else
		set_last_exit_status(128 + WTERMSIG(status));
}

void	wait_for_pipes(pid_t *pid, int cmd_count)
{
	int	i;
	int	status;

	signal(SIGINT, SIG_IGN);
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pid[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFSIGNALED(status))
				handle_pipe_signal_exit(status);
			else if (WIFEXITED(status))
				set_last_exit_status(WEXITSTATUS(status));
			else
				set_last_exit_status(1);
		}
		i++;
	}
	setup_signals();
}
