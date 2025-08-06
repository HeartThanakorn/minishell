/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 19:00:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 18:45:06 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_cmd *cmd)
{
	int		exit_status;
	char	*cmd_path;

	if (is_builtin(cmd->cmd))
	{
		exit_status = execute_builtin(cmd->args);
		set_last_exit_status(exit_status);
	}
	else
	{
		cmd_path = resolve_cmd_path(cmd->cmd);
		if (!cmd_path)
		{
			ft_printf("minishell: %s: command not found\n", cmd->cmd);
			exit(127);
		}
		setup_child_signals();
		if (execve(cmd_path, cmd->args, environ) == -1)
		{
			perror("execve");
			exit(127);
		}
	}
}

static int	cmd_has_redirection(t_cmd *cmd)
{
	return (cmd->infile || cmd->outfile || cmd->here_doc);
}

static void	exec_builtin_with_redir(t_cmd *cmd)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == 0)
	{
		handle_input_redirection(cmd);
		handle_output_redirection(cmd);
		execute_command(cmd);
		exit(0);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		set_last_exit_status(WEXITSTATUS(status));
	}
	else
	{
		perror("fork");
		set_last_exit_status(1);
	}
}

static void	exec_external_with_redir(t_cmd *cmd)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == 0)
	{
		handle_input_redirection(cmd);
		handle_output_redirection(cmd);
		execute_command(cmd);
		exit(0);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		set_last_exit_status(WEXITSTATUS(status));
	}
	else
	{
		perror("fork");
		set_last_exit_status(1);
	}
}

void	exec_single_cmd(t_cmd *cmd, int input_fd, int output_fd)
{
	(void)input_fd;
	(void)output_fd;
	if (!cmd || !cmd->cmd)
		return ;
	if (is_builtin(cmd->cmd))
	{
		if (cmd_has_redirection(cmd))
			exec_builtin_with_redir(cmd);
		else
			execute_command(cmd);
	}
	else
	{
		exec_external_with_redir(cmd);
	}
}
