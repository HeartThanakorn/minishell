/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:54:16 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/06 17:13:28 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_env.h"

int	init_files(t_cmd *cmd)
{
	cmd->in_fd = -1;
	cmd->out_fd = -1;
	if (cmd->infile)
	{
		cmd->in_fd = open(cmd->infile, O_RDONLY);
		if (cmd->in_fd < 0)
			return (ft_putstr_fd("Error: could not open infile\n", 2), 1);
	}
	if (cmd->outfile)
	{
		if (cmd->append)
			cmd->out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			cmd->out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->out_fd < 0)
		{
			if (cmd->in_fd != -1)
				close(cmd->in_fd);
			return (ft_putstr_fd("Error: Could not open outfile\n", 2), 1);
		}
	}
	return (0);
}

static void	child(t_cmd *cmd, t_shell *shell, char **envp)
{
	if (cmd->infile)
		dup2(cmd->in_fd, STDIN_FILENO);
	if (cmd->outfile)
		dup2(cmd->out_fd, STDOUT_FILENO);
	close(cmd->in_fd);
	close(cmd->out_fd);
	if (execve(cmd->path, cmd->args, envp) == -1)
		error_exit(shell, "command not found\n");
}

void	exec_cmd(t_cmd *cmd, t_shell *shell, char **envp)
{
	pid_t	pid;

	if (init_files(cmd))
		error_exit(shell, "file init failed\n");
	pid = fork();
	if (pid == -1)
		error_exit(shell, "fork failed\n");
	if (pid == 0)
	{
		setup_child_signals();
		child(cmd, shell, envp);
	}
	waitpid(pid, NULL, 0);
}
