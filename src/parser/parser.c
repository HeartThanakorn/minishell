/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:48:15 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/06 18:29:55 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "token.h"

static int	parse_redir(t_list **tokens, t_cmd *cmd)
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
	*tokens = (*tokens)->next;
	return (0);
}

static void	parse_args(t_list **tokens, t_cmd *cmd)
{
	t_token	*tok;
	t_list	*args_list;

	args_list = NULL;
	if (!tokens || !*tokens || !(*tokens)->content)
		return ;
	tok = (t_token *)(*tokens)->content;
	if (!tok->value)
		return ;
	cmd->cmd = ft_strdup(tok->value);
	add_arg(&args_list, cmd->cmd);
	*tokens = (*tokens)->next;
	while (*tokens)
	{
		tok = (t_token *)(*tokens)->content;
		if (tok->type != WORD)
			break ;
		process_arg_token(tok, &args_list);
		*tokens = (*tokens)->next;
	}
	cmd->args = list_to_array(args_list);
	ft_lstclear(&args_list, free);
}

static int	handle_token(t_list **tokens, t_cmd *cmd)
{
	t_token	*tok;

	tok = (t_token *)(*tokens)->content;
	if (tok->type == REDIR_IN || tok->type == REDIR_OUT 
		|| tok->type == REDIR_APPEND || tok->type == REDIR_HERE_DOC)
	{
		if (parse_redir(tokens, cmd))
			return (1);
	}
	else if (tok->type == WORD)
	{
		parse_args(tokens, cmd);
		return (0);
	}
	else if (tok->type == PIPE)
	{
		*tokens = (*tokens)->next;
		return (2);
	}
	else
		*tokens = (*tokens)->next;
	return (0);
}

static t_cmd	*create_cmd(t_list **tokens)
{
	int		status;
	t_cmd	*cmd;

	if (!tokens || !*tokens)
		return (NULL);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_init_cmd(cmd);
	while (*tokens)
	{
		status = handle_token(tokens, cmd);
		if (status == 1)
			return (free(cmd), NULL);
		else if (status == 2)
			break ;
	}
	if (!cmd->cmd && !cmd->infile && !cmd->outfile && !cmd->here_doc)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

t_cmd	*parse(t_list *tokens)
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
			new_cmd = create_cmd(&tokens);
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
