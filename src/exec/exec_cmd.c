/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:54:16 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/11 13:15:27 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_files(t_cmd *cmd, t_shell *shell)
{
	cmd->in_fd = -1;
	cmd->out_fd = -1;
	if (init_input_files(cmd, shell) != 0)
		return (1);
	if (init_output_files(cmd) != 0)
		return (1);
	return (0);
}

static void	child(t_cmd *cmd, t_shell *shell, char **envp)
{
	if (cmd->here_doc && cmd->in_fd != -1)
		dup2(cmd->in_fd, STDIN_FILENO);
	if (cmd->infile && cmd->in_fd != -1)
		dup2(cmd->in_fd, STDIN_FILENO);
	if (cmd->outfile && cmd->out_fd != -1)
		dup2(cmd->out_fd, STDOUT_FILENO);
	if (cmd->in_fd != -1)
		close(cmd->in_fd);
	if (cmd->out_fd != -1)
		close(cmd->out_fd);
	exec_child_command(cmd, shell, envp);
}

void	exec_cmd(t_cmd *cmd, t_shell *shell, char **envp)
{
	pid_t	pid;
	int		status;

	if (init_files(cmd, shell))
	{
		set_last_exit_status(1);
		return ;
	}
	pid = fork();
	if (pid == -1)
		error_exit(shell, cmd, "fork failed\n");
	if (pid == 0)
	{
		setup_child_signals();
		child(cmd, shell, envp);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFSIGNALED(status))
		handle_signal_exit(status);
	else if (WIFEXITED(status))
		set_last_exit_status(WEXITSTATUS(status));
	else
		set_last_exit_status(1);
}
