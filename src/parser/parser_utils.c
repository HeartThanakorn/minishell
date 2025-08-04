/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:06:17 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/04 17:44:27 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
		array[i++] = ft_strdup((char *)lst->content);
		lst = lst->next;
	}
	array[i] = NULL;
	return (array);
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	free_cmd_lst(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		free(cmd->cmd);
		free_array(cmd->args);
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		free(cmd);
		cmd = next;
	}
}
