/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 10:53:40 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/11 14:33:30 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_path_s(t_list *env_list)
{
	t_env	*env;

	while (env_list)
	{
		env = (t_env *)env_list->content;
		if (ft_strncmp(env->key, "PATH", 4) == 0)
			return (env->value);
		env_list = env_list->next;
	}
	return (NULL);
}

char	**get_path(t_list *env_list)
{
	char	*path;

	path = get_path_s(env_list);
	if (!path)
		return (NULL);
	return (ft_split(path, ':'));
}

static char	*find_path(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*full_path;

	i = 0;
	temp = NULL;
	full_path = NULL;
	while (paths && paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

int	cmd_path(t_cmd *cmd_list, char **paths)
{
	t_cmd	*temp;

	temp = cmd_list;
	while (temp)
	{
		if (!temp->args || !temp->args[0])
		{
			temp = temp->next;
			continue ;
		}
		if (ft_strchr(temp->args[0], '/'))
		{
			if (access(temp->args[0], X_OK) == 0)
				temp->path = ft_strdup(temp->args[0]);
			else
				temp->path = NULL;
		}
		else
			temp->path = find_path(paths, temp->args[0]);
		temp = temp->next;
	}
	return (0);
}

int	count_commands(t_cmd *cmd_list)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}
