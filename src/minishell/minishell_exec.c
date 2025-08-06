/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:30:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 18:19:21 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_child_process(char *cmd_path, char **args)
{
	setup_child_signals();
	if (execve(cmd_path, args, environ) == -1)
	{
		perror("execve");
		exit(127);
	}
	return (0);
}

int	fork_and_execute(char *cmd_path, char **args)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == 0)
	{
		execute_child_process(cmd_path, args);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	else
	{
		perror("fork");
		return (-1);
	}
	return (0);
}
