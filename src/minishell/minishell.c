/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:19:07 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/08 17:15:03 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "token.h"
#include <stdio.h>
#include <termios.h>

char	*handle_input(void)
{
	char	*input;

	input = readline("minishell$ ");
	if (!input)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (NULL);
	}
	if (input && *input)
		add_history(input);
	return (input);
}

static void	handle_single_builtin(t_cmd *cmd_list, t_shell *shell, char **envp)
{
	int	exit_code;

	if (cmd_list->infile || cmd_list->outfile || cmd_list->here_doc)
		exec_cmd(cmd_list, shell, envp);
	else
	{
		exit_code = execute_builtin(cmd_list->args);
		if (exit_code == -42)
		{
			shell->should_exit = 1;
			shell->exit_status = get_last_exit_status();
		}
		else
			set_last_exit_status(exit_code);
	}
}

void	process_line(char *input, t_shell *shell, char **envp)
{
	t_list	*tokens;
	t_cmd	*cmd_list;

	tokens = tokenize(input);
	if (!tokens)
		return ;
	cmd_list = parse(tokens);
	if (cmd_list)
	{
		shell->cmd_count = count_commands(cmd_list);
		if (shell->paths)
			free_array(shell->paths);
		shell->paths = get_path(shell->env_list);
		cmd_path(cmd_list, shell->paths);
		if (shell->cmd_count <= 0)
			error_exit(shell, cmd_list, "no commands to execute\n");
		if (shell->cmd_count == 1)
		{
			if (is_builtin(cmd_list->cmd))
				handle_single_builtin(cmd_list, shell, envp);
			else
				exec_cmd(cmd_list, shell, envp);
		}
		else if (shell->cmd_count > 1)
			exec_pipe(cmd_list, shell->cmd_count, envp);
		free_cmd_lst(cmd_list);
	}
	free_tokens(tokens);
}

void	minishell_loop(t_shell *shell, char **envp)
{
	char	*input;

	while (!shell->should_exit)
	{
		input = handle_input();
		if (!input)
		{
			shell->should_exit = 1;
			break ;
		}
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			free(input);
			continue ;
		}
		process_line(input, shell, envp);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.should_exit = 0;
	shell.exit_status = 0;
	init_env(&shell, envp);
	shell.paths = get_path(shell.env_list);
	set_last_exit_status(0);
	setup_signals();
	minishell_loop(&shell, envp);
	ft_clean(NULL, &shell);
	return (shell.exit_status);
}
