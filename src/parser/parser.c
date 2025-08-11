/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:48:15 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/11 12:35:11 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "token.h"

static t_cmd	*create_cmd(t_list **tokens, t_shell *shell)
{
	t_cmd	*cmd;

	if (!tokens || !*tokens)
		return (NULL);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_init_cmd(cmd);
	if (process_cmd_tokens(tokens, cmd, shell) == 1)
		return (free(cmd), NULL);
	if (!cmd->cmd && !cmd->infile && !cmd->outfile && !cmd->here_doc)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

t_cmd	*parse(t_list *tokens, t_shell *shell)
{
	t_cmd	*cmd_head;
	t_cmd	*cmd;
	t_cmd	*new_cmd;
	t_token	*tok;

	cmd_head = NULL;
	cmd = NULL;
	while (tokens)
	{
		tok = (t_token *)tokens->content;
		if (tok->type == PIPE)
			tokens = tokens->next;
		else
		{
			new_cmd = create_cmd(&tokens, shell);
			if (!new_cmd)
				return (free_cmd_lst(cmd_head), NULL);
			if (!cmd_head)
				cmd_head = new_cmd;
			else
				cmd->next = new_cmd;
			cmd = new_cmd;
		}
	}
	return (cmd_head);
}
