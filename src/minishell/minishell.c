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
#include "token.h"
#include <stdio.h>

void	print_cmd(t_cmd *cmd)
{
	int	i;

	while (cmd)
	{
		printf("Command: %s\n", cmd->cmd ? cmd->cmd : "NULL");
		printf("Arguments:\n");
		if (cmd->args)
		{
			for (i = 0; cmd->args[i]; i++)
				printf("  arg[%d]: %s\n", i, cmd->args[i]);
		}
		else
			printf("  None\n");

		if (cmd->is_infile)
			printf("Infile: %s\n", cmd->infile);
		if (cmd->append)
			printf("Append Outfile: %s\n", cmd->outfile);
		else if (cmd->outfile)
			printf("Outfile: %s\n", cmd->outfile);
		if (cmd->here_doc)
			printf("Heredoc delimiter: %s\n", cmd->delim);
		printf("------\n");
		cmd = cmd->next;
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
		/*if (g_sinal == SIGINT)
		{
			g_signal = 0;
			free(input);
			continue ;
		}*/
		if (!input)
			break ;
		if (*input)
			add_history(input);
		tokens = tokenize(input);
		cmd_list = parse(tokens);
		print_cmd(cmd_list);
	//	exec_cmds(cmd_list);
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
