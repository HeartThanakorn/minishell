/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:45:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 15:23:04 by tthajan          ###   ########.fr       */
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

void	exec_pipeline(t_cmd *cmd_list)
{
	int		pipefd[2];
	int		input_fd;
	t_cmd	*current;
	pid_t	*pids;
	int		cmd_count;
	int		i;
	int		status;

	cmd_count = count_commands(cmd_list);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return;

	input_fd = STDIN_FILENO;
	current = cmd_list;
	i = 0;

	while (current)
	{
		if (!current->cmd)
		{
			current = current->next;
			continue;
		}

		// Create pipe for all commands except the last one
		if (current->next && current->next->cmd)
		{
			if (create_pipe(pipefd) == -1)
			{
				free(pids);
				return;
			}
		}

		pids[i] = fork();
		if (pids[i] == 0)
		{
			// Child process
			setup_child_signals();

			// Set up input
			if (input_fd != STDIN_FILENO)
			{
				dup2(input_fd, STDIN_FILENO);
				close(input_fd);
			}

			// Set up output
			if (current->next && current->next->cmd)
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
				close(pipefd[0]);
			}

			// Handle redirections for this command
			exec_single_cmd(current, STDIN_FILENO, STDOUT_FILENO);
			exit(0);
		}
		else if (pids[i] > 0)
		{
			// Parent process
			if (input_fd != STDIN_FILENO)
				close(input_fd);

			if (current->next && current->next->cmd)
			{
				close(pipefd[1]);
				input_fd = pipefd[0];
			}
		}
		else
		{
			perror("fork");
			free(pids);
			return;
		}

		current = current->next;
		i++;
	}

	// Wait for all children
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		// The exit status of the pipeline is the exit status of the last command
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				set_last_exit_status(WEXITSTATUS(status));
			else
				set_last_exit_status(1); // Error if not normally exited
		}
		i++;
	}

	free(pids);
}

void	exec_single_cmd(t_cmd *cmd, int input_fd, int output_fd)
{
	int	exit_status;

	(void)input_fd;
	(void)output_fd;
	
	if (!cmd || !cmd->cmd)
		return;

	// Handle input redirection
	if (cmd->here_doc)
	{
		if (handle_heredoc(cmd->delim) == -1)
			return;
	}
	else if (cmd->is_infile && cmd->infile)
	{
		if (redirect_input(cmd->infile) == -1)
			return;
	}

	// Handle output redirection
	if (cmd->outfile)
	{
		if (cmd->append)
		{
			if (redirect_append(cmd->outfile) == -1)
				return;
		}
		else
		{
			if (redirect_output(cmd->outfile) == -1)
				return;
		}
	}

	// Execute command
	if (is_builtin(cmd->cmd))
	{
		exit_status = execute_builtin(cmd->args);
		set_last_exit_status(exit_status);
	}
	else
	{
		exit_status = exec_external_cmd(cmd->args);
		if (exit_status == -1)
		{
			ft_printf("minishell: %s: command not found\n", cmd->cmd);
			set_last_exit_status(127); // Command not found
		}
		else
		{
			set_last_exit_status(exit_status);
		}
	}
}
