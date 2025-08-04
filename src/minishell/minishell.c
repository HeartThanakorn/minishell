/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:19:07 by kmaeda            #+#    #+#             */
/*   Updated: 2025/07/31 13:41:40 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_env.h"

void	minishell_loop(void)
{
	char	*input;
	t_list	*tokens;
	t_cmd	*cmd_list;

	while (1)
	{
		input = readline("minishell> ");
		if (g_sinal == SIGINT)
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
		exec_cmds(cmd_list);
		free(cmd_list);
		free_tokens(tokens);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_env(&shell, envp);
	shell.paths = get_path(shell.env_list);
	minishell_loop();
	return (0);
}
