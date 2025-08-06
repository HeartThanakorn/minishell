/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:19:07 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/06 15:42:11 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "token.h"
#include <stdio.h>
#include <termios.h>

void	minishell_loop(void)
{
	char	*input;
	t_list	*tokens;
	t_cmd	*cmd_list;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)  // ctrl-D (EOF) - exit immediately
			break ;
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			free(input);
			continue ;
		}
		if (*input)
			add_history(input);
		tokens = tokenize(input);
		cmd_list = parse(tokens);
		exec_cmds(cmd_list);
		free(cmd_list);
		free_tokens(tokens);
		free(input);
	}
}

int	exec_external_cmd(char **args)
{
	int		pid;
	int		status;
	char	*cmd_path;
	char	*path_env;
	char	**paths;
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
	else
	{
		// Parse PATH environment variable
		path_env = getenv("PATH");
		if (path_env)
		{
			paths = ft_split(path_env, ':');
			if (paths)
			{
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
				// Free paths array
				i = 0;
				while (paths[i])
					free(paths[i++]);
				free(paths);
			}
		}
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

void	exec_cmds(t_cmd *cmd_list)
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
		exec_pipeline(cmd_list);
	}
	else
	{
		// Single command, execute normally
		current = cmd_list;
		exec_single_cmd(current, STDIN_FILENO, STDOUT_FILENO);
	}
}

int	main(int argc, char **argv, char **envp)
{
	struct termios	term;
	
	(void)argc;
	(void)argv;
	(void)envp;
	
	// Configure terminal to not echo control characters
	if (tcgetattr(STDIN_FILENO, &term) == 0)
	{
#ifdef ECHOCTL
		term.c_lflag &= ~ECHOCTL;  // Don't echo ^C, ^\, etc.
#endif
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
	
	// Initialize exit status to 0
	set_last_exit_status(0);
	
	setup_signals();
	minishell_loop();
	return (0);
}
