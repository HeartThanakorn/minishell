/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:24:39 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/11 13:15:28 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	first_child(t_cmd *cmd, t_shell *shell, char **envp)
{
	close(cmd->fd[0]);
	if (cmd->in_fd != STDIN_FILENO)
	{
		dup2(cmd->in_fd, STDIN_FILENO);
		close(cmd->in_fd);
	}
	dup2(cmd->fd[1], STDOUT_FILENO);
	close(cmd->fd[1]);
	setup_child_signals();
	execute_command(cmd, shell, envp);
}

static void	last_child(t_cmd *cmd, t_shell *shell, char **envp)
{
	dup2(cmd->prev_fd, STDIN_FILENO);
	close(cmd->prev_fd);
	if (cmd->out_fd != STDOUT_FILENO)
	{
		dup2(cmd->out_fd, STDOUT_FILENO);
		close(cmd->out_fd);
	}
	setup_child_signals();
	execute_command(cmd, shell, envp);
}

static void	middle_child(t_cmd *cmd, int i, t_shell *shell, char **envp)
{
	(void)i;
	dup2(cmd->prev_fd, STDIN_FILENO);
	close(cmd->prev_fd);
	dup2(cmd->fd[1], STDOUT_FILENO);
	close(cmd->fd[0]);
	close(cmd->fd[1]);
	setup_child_signals();
	execute_command(cmd, shell, envp);
}

static void	pipex(t_cmd *cmd, pid_t *pid, int i, t_shell *shell, char **envp)
{
	if (cmd->next && pipe(cmd->fd) == -1)
		error_exit(NULL, cmd, "pipe failed\n");
	pid[i] = fork();
	if (pid[i] == -1)
		error_exit(NULL, cmd, "fork failed\n");
	if (pid[i] == 0)
	{
		if (i == 0)
			first_child(cmd, shell, envp);
		else if (!cmd->next)
			last_child(cmd, shell, envp);
		else
			middle_child(cmd, i, shell, envp);
	}
	if (cmd->prev_fd != -1)
		close(cmd->prev_fd);
	if (cmd->next)
	{
		close(cmd->fd[1]);
		cmd->next->prev_fd = cmd->fd[0];
	}
}

void	exec_pipe(t_cmd *cmd_list, int cmd_count, t_shell *shell, char **envp)
{
	int		i;
	pid_t	*pid;
	t_cmd	*cmd;

	i = 0;
	cmd = cmd_list;
	cmd->prev_fd = -1;
	pid = malloc(sizeof(pid_t) * cmd_count);
	if (!pid)
		error_exit(NULL, cmd_list, "malloc failed\n");
	while (cmd)
	{
		if (init_files(cmd, shell))
		{
			free(pid);
			set_last_exit_status(1);
			return ;
		}
		pipex(cmd, pid, i, shell, envp);
		cmd = cmd->next;
		i++;
	}
	wait_for_pipes(pid, cmd_count);
	free(pid);
}
