/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 10:40:25 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/07 18:45:21 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "token.h"
#include "cmd.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

void	free_env(void *content)
{
	t_env	*env;

	env = (t_env *)content;
	if (env->key)
		free(env->key);
	if (env->value)
		free(env->value);
	free(env);
}

void	ft_clean(t_cmd *cmd, t_shell *shell)
{
	int	i;

	i = 0;
	if (cmd)
	{
		if (cmd->in_fd >= 0)
			close(cmd->in_fd);
		if (cmd->out_fd >= 0)
			close(cmd->out_fd);
		if (cmd->args)
			free_array(cmd->args);
		if (cmd->path)
			free(cmd->path);
		free_cmd_lst(cmd);
	}
	if (shell)
	{
		if (shell->paths)
			free_array(shell->paths);
		if (shell->env_list)
			ft_lstclear(&shell->env_list, free_env);
	}
}

void	error_exit(t_shell *shell, t_cmd *cmd, char *msg)
{
	perror(msg);
	ft_clean(cmd, shell);
	exit(1);
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
