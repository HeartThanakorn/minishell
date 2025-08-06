/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:19:07 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/06 18:19:21 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "token.h"
#include <stdio.h>
#include <termios.h>

#ifdef ECHOCTL
# define ECHO_CTL_FLAG ECHOCTL
#else
# define ECHO_CTL_FLAG 0
#endif

static void	configure_terminal(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == 0)
	{
		if (ECHO_CTL_FLAG)
			term.c_lflag &= ~ECHO_CTL_FLAG;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
}

void	minishell_loop(void)
{
	char	*input;
	t_list	*tokens;
	t_cmd	*cmd_list;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
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
		free_cmd_lst(cmd_list);
		free_tokens(tokens);
		free(input);
	}
}

int	exec_external_cmd(char **args)
{
	char	*cmd_path;
	int		result;

	if (!args || !args[0])
		return (-1);
	cmd_path = resolve_cmd_path(args[0]);
	if (!cmd_path)
		return (-1);
	result = fork_and_execute(cmd_path, args);
	if (cmd_path != args[0])
		free(cmd_path);
	return (result);
}

void	exec_cmds(t_cmd *cmd_list)
{
	t_cmd	*current;
	int		cmd_count;

	if (!cmd_list)
		return ;
	cmd_count = count_commands(cmd_list);
	if (cmd_count > 1)
	{
		exec_pipeline(cmd_list);
	}
	else
	{
		current = cmd_list;
		exec_single_cmd(current, STDIN_FILENO, STDOUT_FILENO);
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	configure_terminal();
	set_last_exit_status(0);
	setup_signals();
	minishell_loop();
	return (0);
}
