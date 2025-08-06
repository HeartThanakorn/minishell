/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:19:07 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/06 11:30:40 by tthajan          ###   ########.fr       */
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
	int		exit_status;

	current = cmd_list;
	while (current)
	{
		if (!current->cmd)
		{
			current = current->next;
			continue;
		}
		
		// Handle input redirection
		if (current->is_infile)
		{
			if (current->here_doc)
			{
				if (handle_heredoc(current->delim) == -1)
				{
					current = current->next;
					continue;
				}
			}
			else if (current->infile)
			{
				if (redirect_input(current->infile) == -1)
				{
					current = current->next;
					continue;
				}
			}
		}
		
		// Handle output redirection
		if (current->outfile)
		{
			if (current->append)
			{
				if (redirect_append(current->outfile) == -1)
				{
					current = current->next;
					continue;
				}
			}
			else
			{
				if (redirect_output(current->outfile) == -1)
				{
					current = current->next;
					continue;
				}
			}
		}
		
		// Execute command
		if (is_builtin(current->cmd))
		{
			exit_status = execute_builtin(current->args);
			(void)exit_status; // TODO: Handle exit status properly
		}
		else
		{
			exit_status = exec_external_cmd(current->args, shell->paths);
			if (exit_status == -1)
			{
				ft_printf("minishell: %s: command not found\n", current->cmd);
			}
		}
		
		current = current->next;
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
