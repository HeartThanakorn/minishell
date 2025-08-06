/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:00:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 18:19:21 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_paths_array(char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

static char	*check_path_dir(char *dir, char *cmd)
{
	char	*cmd_path;
	char	*full_path;

	cmd_path = ft_strjoin(dir, "/");
	if (!cmd_path)
		return (NULL);
	full_path = ft_strjoin(cmd_path, cmd);
	free(cmd_path);
	if (full_path && access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*find_cmd_in_path(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*result;
	int		i;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		result = check_path_dir(paths[i], cmd);
		if (result)
		{
			free_paths_array(paths);
			return (result);
		}
		i++;
	}
	free_paths_array(paths);
	return (NULL);
}

char	*resolve_cmd_path(char *cmd)
{
	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (cmd);
	return (find_cmd_in_path(cmd));
}
