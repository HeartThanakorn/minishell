/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:30:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/07 18:35:25 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "token.h"

int	parse_redir(t_list **tokens, t_cmd *cmd)
{
	t_token	*tok;
	char	*expanded_filename;
	int		redir_type;

	tok = (t_token *)(*tokens)->content;
	redir_type = tok->type;
	set_redir_flags(cmd, redir_type);
	*tokens = (*tokens)->next;
	if (!*tokens)
		return (1);
	tok = (t_token *)(*tokens)->content;
	if (tok->type != WORD)
		return (1);
	expanded_filename = expand_env_vars_quoted(tok->value, tok->quote_type);
	if (!expanded_filename)
		expanded_filename = ft_strdup(tok->value);
	assign_redir_file(cmd, redir_type, expanded_filename);
	*tokens = (*tokens)->next;
	return (0);
}

void	parse_args(t_list **tokens, t_cmd *cmd)
{
	t_token	*tok;
	t_list	*args_list;
	char	*expanded_cmd;

	args_list = NULL;
	if (!tokens || !*tokens || !(*tokens)->content)
		return ;
	tok = (t_token *)(*tokens)->content;
	if (!tok->value)
		return ;
	expanded_cmd = expand_env_vars_quoted(tok->value, tok->quote_type);
	if (!expanded_cmd)
		expanded_cmd = ft_strdup(tok->value);
	cmd->cmd = expanded_cmd;
	add_arg(&args_list, cmd->cmd, 0);
	*tokens = (*tokens)->next;
	process_word_tokens(tokens, &args_list);
	cmd->args = list_to_array(args_list);
	ft_lstclear(&args_list, free);
}

void	process_word_tokens(t_list **tokens, t_list **args_list)
{
	t_token	*tok;

	while (*tokens)
	{
		tok = (t_token *)(*tokens)->content;
		if (tok->type != WORD)
			break ;
		add_arg(args_list, tok->value, tok->quote_type);
		*tokens = (*tokens)->next;
	}
}

int	handle_token(t_list **tokens, t_cmd *cmd)
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

int	process_cmd_tokens(t_list **tokens, t_cmd *cmd)
{
	int	status;

	while (*tokens)
	{
		status = handle_token(tokens, cmd);
		if (status == 1)
			return (1);
		else if (status == 2)
			break ;
	}
	return (0);
}
