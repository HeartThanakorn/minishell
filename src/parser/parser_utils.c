/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:06:17 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/07 10:29:36 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

int	cmd_list_size(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

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
	int		count;
	int		i;
	char	**array;

	i = 0;
	count = ft_lstsize(lst);
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	while (lst)
	{
		array[i] = ft_strdup((char *)lst->content);
		if (!array[i])
		{
			free_array(array);
			return (NULL);
		}
		i++;
		lst = lst->next;
	}
	array[i] = NULL;
	return (array);
}
