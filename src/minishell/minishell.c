/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:19:07 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/07 10:34:26 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "token.h"
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>

char	*handle_input(void)
{
	char	*input;

	input = readline("minishell> ");
	if (input && *input)
		add_history(input);
	return (input);
}

void	process_line(char *input, t_shell *shell, char **envp)
{
	t_list	*tokens;
	t_cmd	*cmd_list;

	tokens = tokenize(input);
	cmd_list = parse(tokens);
	if (cmd_list)
	{
		shell->cmd_count = cmd_list_size(cmd_list);
		cmd_path(cmd_list, shell->paths);
		if (shell->cmd_count <= 0)
			error_exit(shell, cmd_list, "no commands to execute\n");
		if (shell->cmd_count == 1)
			exec_cmd(cmd_list, shell, envp);
		else if (shell->cmd_count > 1)
			exec_pipe(cmd_list, shell, envp);
		free_cmd_lst(cmd_list);
	}
	free_tokens(tokens);
}

void	minishell_loop(t_shell *shell, char **envp)
{
	char	*input;

	while (1)
	{
		input = handle_input();
		if (!input)
			break ;
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
	struct termios	term;

	(void)argc;
	(void)argv;
	if (tcgetattr(STDIN_FILENO, &term) == 0)
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	init_env(&shell, envp);
	shell.paths = get_path(shell.env_list);
	set_last_exit_status(0);
	setup_signals();
	minishell_loop(&shell, envp);
	free_array(shell.paths);
	ft_lstclear(&shell.env_list, free_env);
	return (0);
}
