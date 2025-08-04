/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:19:07 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/04 18:05:04 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_env.h"
#include "minishell.h"
#include "token.h"

// Global signal variable
int	g_signal = 0;

// Free arguments array
void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

// Check if tokens contain pipe operators
int	has_pipes(t_list *tokens)
{
	t_list	*current;

	current = tokens;
	while (current)
	{
		char *token = (char *)current->content;
		if (ft_strncmp(token, "|", 2) == 0 && ft_strlen(token) == 1)
			return (1);
		current = current->next;
	}
	return (0);
}

// Split tokens by pipe operators
t_list	**split_tokens_by_pipes(t_list *tokens, int *cmd_count)
{
	t_list	**commands;
	t_list	*current;
	t_list	*cmd_tokens;
	int		count;
	int		i;

	// Count number of commands (pipe count + 1)
	count = 1;
	current = tokens;
	while (current)
	{
		char *token = (char *)current->content;
		if (ft_strncmp(token, "|", 2) == 0 && ft_strlen(token) == 1)
			count++;
		current = current->next;
	}
	
	*cmd_count = count;
	commands = malloc(sizeof(t_list *) * count);
	if (!commands)
		return (NULL);
	
	// Split tokens into separate command lists
	current = tokens;
	cmd_tokens = NULL;
	i = 0;
	
	while (current && i < count)
	{
		char *token = (char *)current->content;
		
		if (ft_strncmp(token, "|", 2) == 0 && ft_strlen(token) == 1)
		{
			// End of current command, save it and start new one
			commands[i] = cmd_tokens;
			cmd_tokens = NULL;
			i++;
		}
		else
		{
			// Add token to current command
			t_list *new_node = ft_lstnew(ft_strdup(token));
			if (!new_node)
			{
				// Cleanup on error
				while (i >= 0)
				{
					free_tokens(commands[i]);
					i--;
				}
				free(commands);
				return (NULL);
			}
			ft_lstadd_back(&cmd_tokens, new_node);
		}
		current = current->next;
	}
	
	// Save the last command
	if (i < count)
		commands[i] = cmd_tokens;
	
	return (commands);
}

// Create pipeline structure from tokens
t_pipeline	*create_pipeline(t_list *tokens, t_shell *shell)
{
	t_pipeline	*pipeline;
	t_list		**cmd_token_lists;
	int			i;

	pipeline = malloc(sizeof(t_pipeline));
	if (!pipeline)
		return (NULL);
	
	// Split tokens by pipes
	cmd_token_lists = split_tokens_by_pipes(tokens, &pipeline->cmd_count);
	if (!cmd_token_lists)
	{
		free(pipeline);
		return (NULL);
	}
	
	// Convert token lists to command arrays
	pipeline->commands = malloc(sizeof(char **) * pipeline->cmd_count);
	if (!pipeline->commands)
	{
		free(cmd_token_lists);
		free(pipeline);
		return (NULL);
	}
	
	i = 0;
	while (i < pipeline->cmd_count)
	{
		pipeline->commands[i] = process_redirections_and_args(cmd_token_lists[i], shell);
		if (!pipeline->commands[i])
		{
			// Cleanup on error
			while (--i >= 0)
				free_args(pipeline->commands[i]);
			free(pipeline->commands);
			free(cmd_token_lists);
			free(pipeline);
			return (NULL);
		}
		free_tokens(cmd_token_lists[i]);
		i++;
	}
	
	free(cmd_token_lists);
	
	// Allocate pipe arrays
	if (pipeline->cmd_count > 1)
	{
		pipeline->pipes = malloc(sizeof(int *) * (pipeline->cmd_count - 1));
		if (!pipeline->pipes)
		{
			free_pipeline(pipeline);
			return (NULL);
		}
		i = 0;
		while (i < pipeline->cmd_count - 1)
		{
			pipeline->pipes[i] = malloc(sizeof(int) * 2);
			if (!pipeline->pipes[i] || pipe(pipeline->pipes[i]) == -1)
			{
				while (--i >= 0)
					free(pipeline->pipes[i]);
				free(pipeline->pipes);
				free_pipeline(pipeline);
				return (NULL);
			}
			i++;
		}
	}
	else
		pipeline->pipes = NULL;
	
	// Allocate PID array
	pipeline->pids = malloc(sizeof(pid_t) * pipeline->cmd_count);
	if (!pipeline->pids)
	{
		free_pipeline(pipeline);
		return (NULL);
	}
	
	return (pipeline);
}

// Free pipeline structure
void	free_pipeline(t_pipeline *pipeline)
{
	int	i;

	if (!pipeline)
		return ;
	
	if (pipeline->commands)
	{
		i = 0;
		while (i < pipeline->cmd_count)
		{
			if (pipeline->commands[i])
				free_args(pipeline->commands[i]);
			i++;
		}
		free(pipeline->commands);
	}
	
	if (pipeline->pipes)
	{
		i = 0;
		while (i < pipeline->cmd_count - 1)
		{
			if (pipeline->pipes[i])
			{
				close(pipeline->pipes[i][0]);
				close(pipeline->pipes[i][1]);
				free(pipeline->pipes[i]);
			}
			i++;
		}
		free(pipeline->pipes);
	}
	
	if (pipeline->pids)
		free(pipeline->pids);
	
	free(pipeline);
}

// Execute pipeline
int	execute_pipeline(t_pipeline *pipeline, t_shell *shell)
{
	int	i;
	int	status;
	int	last_exit_status;

	if (!pipeline || pipeline->cmd_count == 0)
		return (1);
	
	// Fork and execute each command
	for (i = 0; i < pipeline->cmd_count; i++)
	{
		pipeline->pids[i] = fork();
		if (pipeline->pids[i] == 0)
		{
			// Child process - restore default signal handling
			setup_child_signals();
			
			int input_fd = STDIN_FILENO;
			int output_fd = STDOUT_FILENO;
			
			// Set up input redirection
			if (i > 0)
			{
				input_fd = pipeline->pipes[i - 1][0];
				dup2(input_fd, STDIN_FILENO);
			}
			
			// Set up output redirection
			if (i < pipeline->cmd_count - 1)
			{
				output_fd = pipeline->pipes[i][1];
				dup2(output_fd, STDOUT_FILENO);
			}
			
			// Close all pipe file descriptors in child
			int j = 0;
			while (j < pipeline->cmd_count - 1)
			{
				close(pipeline->pipes[j][0]);
				close(pipeline->pipes[j][1]);
				j++;
			}
			
			// Execute command
			if (is_builtin(pipeline->commands[i][0]))
			{
				exit(execute_builtin(pipeline->commands[i]));
			}
			else
			{
				// For non-builtin commands
				write(STDERR_FILENO, "minishell: command not found: ", 30);
				write(STDERR_FILENO, pipeline->commands[i][0], ft_strlen(pipeline->commands[i][0]));
				write(STDERR_FILENO, "\n", 1);
				exit(127);
			}
		}
		else if (pipeline->pids[i] < 0)
		{
			// Fork failed
			perror("fork");
			return (1);
		}
	}
	
	// Parent process: close all pipe file descriptors
	for (i = 0; i < pipeline->cmd_count - 1; i++)
	{
		close(pipeline->pipes[i][0]);
		close(pipeline->pipes[i][1]);
	}
	
	// Wait for all children
	last_exit_status = 0;
	for (i = 0; i < pipeline->cmd_count; i++)
	{
		waitpid(pipeline->pids[i], &status, 0);
		if (i == pipeline->cmd_count - 1) // Last command determines exit status
			last_exit_status = WEXITSTATUS(status);
	}
	
	shell->last_exit_status = last_exit_status;
	return (last_exit_status);
}

// Process redirections and return command arguments
char	**process_redirections_and_args(t_list *tokens, t_shell *shell)
{
	t_list	*current;
	t_list	*cmd_tokens;
	char	**args;

	current = tokens;
	cmd_tokens = NULL;
	
	while (current)
	{
		char *token = (char *)current->content;
		
		if (ft_strncmp(token, ">", 2) == 0 && ft_strlen(token) == 1)
		{
			// Output redirection
			current = current->next;
			if (current && redirect_output((char *)current->content) == -1)
				return (NULL);
			current = current->next;  // Skip filename
		}
		else if (ft_strncmp(token, "<", 2) == 0 && ft_strlen(token) == 1)
		{
			// Input redirection
			current = current->next;
			if (current && redirect_input((char *)current->content) == -1)
				return (NULL);
			current = current->next;  // Skip filename
		}
		else if (ft_strncmp(token, ">>", 3) == 0 && ft_strlen(token) == 2)
		{
			// Append redirection
			current = current->next;
			if (current && redirect_append((char *)current->content) == -1)
				return (NULL);
			current = current->next;  // Skip filename
		}
		else if (ft_strncmp(token, "<<", 3) == 0 && ft_strlen(token) == 2)
		{
			// Here document
			current = current->next;
			if (current && handle_heredoc((char *)current->content) == -1)
				return (NULL);
			current = current->next;  // Skip delimiter
		}
		else
		{
			// Regular command argument
			add_token(&cmd_tokens, ft_strdup(token));
			current = current->next;
		}
	}
	
	// Convert command tokens to array
	args = list_to_array(cmd_tokens, shell);
	free_tokens(cmd_tokens);
	return (args);
}

// Execute a simple command
void	execute_command(char *input, t_shell *shell)
{
	t_list		*tokens;
	t_pipeline	*pipeline;
	char		**args;
	int			exit_status;
	int			original_stdout;
	int			original_stdin;

	if (!input || !*input)
		return ;
	tokens = tokenize(input);
	if (!tokens)
	{
		// Tokenization failed (likely due to unclosed quotes)
		printf("minishell: syntax error: unclosed quotes\n");
		shell->last_exit_status = 2;
		return ;
	}
	
	// Save original file descriptors
	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	
	// Check if we have pipes
	if (has_pipes(tokens))
	{
		// Handle pipeline
		pipeline = create_pipeline(tokens, shell);
		if (!pipeline)
		{
			free_tokens(tokens);
			dup2(original_stdout, STDOUT_FILENO);
			dup2(original_stdin, STDIN_FILENO);
			close(original_stdout);
			close(original_stdin);
			return ;
		}
		
		execute_pipeline(pipeline, shell);
		free_pipeline(pipeline);
	}
	else
	{
		// Handle single command (no pipes)
		args = process_redirections_and_args(tokens, shell);
		if (!args)
		{
			free_tokens(tokens);
			dup2(original_stdout, STDOUT_FILENO);
			dup2(original_stdin, STDIN_FILENO);
			close(original_stdout);
			close(original_stdin);
			return ;
		}
		
		if (is_builtin(args[0]))
		{
			exit_status = execute_builtin(args);
			shell->last_exit_status = exit_status;
		}
		else
		{
			// For non-builtin commands, set exit status to 127 (command not found)
			shell->last_exit_status = 127;
		}
		
		free_args(args);
	}
	
	// Restore original file descriptors
	dup2(original_stdout, STDOUT_FILENO);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdout);
	close(original_stdin);
	
	free_tokens(tokens);
}

void	minishell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (g_signal == SIGINT)
		{
			shell->last_exit_status = 130; // Set exit status for SIGINT
			g_signal = 0;
		}
		if (!input) // Ctrl-D (EOF)
		{
			break ; // Exit immediately on EOF
		}
		if (*input)
		{
			add_history(input);
			execute_command(input, shell);
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	setup_signals();
	init_env(&shell, envp);
	shell.paths = get_path(shell.env_list);
	minishell_loop(&shell);
	cleanup_shell(&shell);
	return (0);
}
