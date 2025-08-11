/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:55:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/11 15:57:52 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_parent_dir(char *current_path)
{
	char	*last_slash;
	char	*parent_path;

	last_slash = ft_strrchr(current_path, '/');
	if (last_slash && last_slash != current_path)
	{
		parent_path = ft_substr(current_path, 0, last_slash - current_path);
		return (parent_path);
	}
	else
		return (ft_strdup("/"));
}

char	*get_logical_path(char *path, t_shell *shell)
{
	t_env	*pwd_env;

	if (path[0] == '/')
		return (ft_strdup(path));
	else if (ft_strncmp(path, "..", 2) == 0 && path[2] == '\0')
	{
		pwd_env = find_env_var(shell->env_list, "PWD");
		if (pwd_env && pwd_env->value)
			return (get_parent_dir(pwd_env->value));
		else
			return (getcwd(NULL, 0));
	}
	else
		return (getcwd(NULL, 0));
}

void	update_pwd_vars(char *old_pwd, char *new_logical_path, t_shell *shell)
{
	char	*new_pwd;

	if (old_pwd)
	{
		set_env_value(shell->env_list, "OLDPWD", old_pwd);
		free(old_pwd);
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
		free(new_pwd);
	set_env_value(shell->env_list, "PWD", new_logical_path);
}
