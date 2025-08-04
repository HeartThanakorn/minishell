/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:48:15 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/04 18:40:33 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	ft_init_cmd(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->next = NULL;
	cmd->is_infile = 0;
	cmd->here_doc = 0;
	cmd->append = 0;
}

int	parse_redir(t_list **tokens, t_cmd *cmd)
{
	t_token	*tok;

	tok = (t_token *)(*tokens)->content;
	if (tok->type == REDIR_IN)
		cmd->is_infile = 1;
	else if (tok->type == REDIR_APPEND)
		cmd->append = 1;
	else if (tok->type == REDIR_HERE_DOC)
		cmd->here_doc = 1;
	*tokens = (*tokens)->next;
	if (!*tokens)
		return (1);
	tok = (t_token *)(*tokens)->content;
	if (tok->type != WORD)
		return (1);
	if (cmd->is_infile)
		cmd->infile = ft_strdup(tok->value);
	else if (cmd->here_doc)
		cmd->delim = ft_strdup(tok->value);
	else
		cmd->outfile = ft_strdup(tok->value);
	return (0);
}

void	parse_args(t_list **tokens, t_cmd *cmd)
{
	t_token	*tok;
	t_list	*args_list;
	t_list	*new_node;

	args_list = NULL;
	if (!tokens || !*tokens)
		return ;
	tok = (t_token *)(*tokens)->content;
	cmd->cmd = ft_strdup(tok->value);
	new_node = ft_lstnew(ft_strdup(cmd->cmd));
	ft_lstadd_back(&args_list, new_node);
	*tokens = (*tokens)->next;
	while (*tokens)
	{
		tok = (t_token *)(*tokens)->content;
		if (tok->type != WORD)
			break ;
		new_node = ft_lstnew(ft_strdup(tok->value));
		ft_lstadd_back(&args_list, new_node);
		*tokens = (*tokens)->next;
	}
	cmd->args = list_to_array(args_list);
	ft_lstclear(&args_list, free);
	return ;
}

t_cmd	*create_cmd(t_list **tokens)
{
	t_cmd	*cmd;
	t_token	*tok;

	if (!tokens || !*tokens)
		return (NULL);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_init_cmd(cmd);
	tok = (t_token *)(*tokens)->content;
	if (tok->type == REDIR_IN || tok->type == REDIR_OUT 
		|| tok->type == REDIR_APPEND || tok->type == REDIR_HERE_DOC)
	{
		if (parse_redir(tokens, cmd))
			return (NULL);
	}
	else if (tok->type == WORD)
		parse_args(tokens, cmd);
	else if (tok->type == PIPE)
		*tokens = (*tokens)->next;
	return (cmd);
}

t_cmd	*parse(t_list *tokens)
{
	t_cmd	*cmd_head;
	t_cmd	*cmd;
	t_cmd	*new_cmd;

	cmd_head = NULL;
	cmd = NULL;
	while (tokens)
	{
		new_cmd = create_cmd(&tokens);
		if (!new_cmd)
		{
			free_cmd_lst(cmd_head);
			return (NULL);
		}
		if (!cmd_head)
			cmd_head = new_cmd;
		else
			cmd->next = new_cmd;
		cmd = new_cmd;
		if (tokens && ((t_token *)tokens->content)->type == PIPE)
			tokens = tokens->next;
	}
	return (cmd_head);
}
