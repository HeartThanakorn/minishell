/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:19:07 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/06 14:21:07 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_env.h"
#include "minishell.h"
#include "token.h"
#include <stdio.h>

void	minishell_loop(t_shell *shell)
{
	char	*input;
	t_list	*tokens;
	t_cmd	*cmd_list;

	while (1)
	{
		input = readline("minishell> ");
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			free(input);
			continue ;
		}
		if (!input)
			break ;
		if (*input)
			add_history(input);
		tokens = tokenize(input);
		cmd_list = parse(tokens);
		exec_cmds(cmd_list, shell);
		free(cmd_list);
		free_tokens(tokens);
		free(input);
	}
}

int	exec_external_cmd(char **args, char **paths)
{
	int		pid;
	int		status;
	char	*cmd_path;
	int		i;

	if (!args || !args[0])
		return (-1);
	
	// Try to find the command in PATH
	cmd_path = NULL;
	if (ft_strchr(args[0], '/'))
	{
		// Command contains '/', use as is
		cmd_path = args[0];
	}
	else if (paths)
	{
		// Search in PATH
		i = 0;
		while (paths[i])
		{
			cmd_path = ft_strjoin(paths[i], "/");
			if (cmd_path)
			{
				char *full_path = ft_strjoin(cmd_path, args[0]);
				free(cmd_path);
				if (full_path && access(full_path, X_OK) == 0)
				{
					cmd_path = full_path;
					break;
				}
				free(full_path);
			}
			i++;
		}
		if (!paths[i])
			cmd_path = NULL;
	}
	
	if (!cmd_path)
		return (-1);
		
	pid = fork();
	if (pid == 0)
	{
		// Child process
		setup_child_signals();
		if (execve(cmd_path, args, environ) == -1)
		{
			perror("execve");
			exit(127);
		}
	}
	else if (pid > 0)
	{
		// Parent process
		waitpid(pid, &status, 0);
		if (cmd_path != args[0])
			free(cmd_path);
		return (WEXITSTATUS(status));
	}
	else
	{
		perror("fork");
		if (cmd_path != args[0])
			free(cmd_path);
		return (-1);
	}
	return (0);
}

void	exec_cmds(t_cmd *cmd_list, t_shell *shell)
{
	t_cmd	*current;
	int		cmd_count;

	if (!cmd_list)
		return;

	// Count commands to see if we have a pipeline
	cmd_count = count_commands(cmd_list);
	
	if (cmd_count > 1)
	{
		// We have a pipeline, execute with pipes
		exec_pipeline(cmd_list, shell);
	}
	else
	{
		// Single command, execute normally
		current = cmd_list;
		exec_single_cmd(current, shell, STDIN_FILENO, STDOUT_FILENO);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_env(&shell, envp);
	shell.paths = get_path(shell.env_list);
	setup_signals();
	minishell_loop(&shell);
	return (0);
}
