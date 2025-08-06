/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:06:17 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/06 18:55:03 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_cmd(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->delim = NULL;
	cmd->next = NULL;
	cmd->is_infile = 0;
	cmd->here_doc = 0;
	cmd->append = 0;
}

void	add_arg(t_list **args_list, char *value)
{
	t_list	*new_node;

	new_node = ft_lstnew(ft_strdup(value));
	if (!new_node)
		return ;
	ft_lstadd_back(args_list, new_node);
}

char	**list_to_array(t_list *lst)
{
	int		size;
	char	**array;
	int		i;

	size = ft_lstsize(lst);
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (lst)
	{
		array[i] = ft_strdup((char *)lst->content);
		if (!array[i])
			return (NULL);
		lst = lst->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

void	process_arg_token(t_token *tok, t_list **args_list)
{
	char	*expanded;

	if (tok->quote_type != SINGLE_QUOTE)
	{
		expanded = expand_env_vars(tok->value);
		if (expanded)
		{
			add_arg(args_list, expanded);
			free(expanded);
		}
		else
			add_arg(args_list, tok->value);
	}
	else
		add_arg(args_list, tok->value);
}

void	free_cmd_lst(t_cmd *cmd)
{
	t_cmd	*next;
	int		i;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->cmd)
			free(cmd->cmd);
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		if (cmd->delim)
			free(cmd->delim);
		free(cmd);
		cmd = next;
	}
}
