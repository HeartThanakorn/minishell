/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:24:39 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/06 17:52:50 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_env.h"

static void	first_child(t_cmd *cmd, char **envp)
{
	close(cmd->fd[0]);
	dup2(cmd->infile, STDIN_FILENO);
	close(cmd->infile);
	dup2(cmd->fd[1], STDOUT_FILENO);
	close(cmd->fd[1]);
	if (execve(cmd->path, cmd->args, envp) == -1)
		error_exit(cmd, "command not found\n");
}

static void	last_child(t_cmd *cmd, char **envp)
{
	dup2(cmd->prev_fd, STDIN_FILENO);
	close(cmd->prev_fd);
	dup2(cmd->outfile, STDOUT_FILENO);
	close(cmd->outfile);
	if (execve(cmd->path, cmd->args, envp) == -1)
		error_exit(cmd, "command not found\n");
}

static void	middle_child(t_cmd *cmd, int i, char **envp)
{
	dup2(cmd->prev_fd, STDIN_FILENO);
	close(cmd->prev_fd);
	dup2(cmd->fd[1], STDOUT_FILENO);
	close(cmd->fd[0]);
	close(cmd->fd[1]);
	if (execve(cmd->path, cmd->args, envp) == -1)
		error_exit(cmd, "command not found\n");
}

static void	pipex(t_cmd *cmd, pid_t *pid, int i, char **envp)
{
	if (cmd->next && pipe(cmd->fd) == -1)
		error_exit(cmd, "pipe failed\n");
	pid[i] = fork();
	if (pid[i] == -1)
		error_exit(cmd, "fork failed\n");
	if (pid[i] == 0)
	{
		if (i == 0)
			first_child(cmd, envp);
		else if (!cmd->next)
			last_child(cmd, envp);
		else
			middle_child(cmd, i, envp);
	}
	if (cmd->prev_fd != -1)
		close(cmd->prev_fd);
	if (cmd->next)
	{
		close(cmd->fd[1]);
		cmd->next->prev_fd = cmd->fd[0];
	}
}

void	exec_pipe(t_cmd *cmd_list, int cmd_count, char **envp)
{
	int		i;
	pid_t	*pid;
	t_cmd	*cmd;

	i = 0;
	cmd = cmd_list;
	cmd->prev_fd = -1;
	pid = malloc(sizeof(pid_t) * cmd_count);
	if (!pid)
		error_exit(cmd, "malloc failed\n");
	while (cmd)
	{
		if (init_files(cmd))
			error_exit(cmd, "file init failed\n");
		pipex(cmd, pid, i, envp);
		cmd = cmd->next;
		i++;
	}
	i = 0;
	while (i < cmd_count)
		waitpid(pid[i++], NULL, 0);
	free(pid);
}
