/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:50:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/11 15:57:52 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void	set_env_value(t_list *env_list, char *key, char *value)
{
	t_env	*env_var;

	env_var = find_env_var(env_list, key);
	if (env_var)
	{
		free(env_var->value);
		env_var->value = ft_strdup(value);
	}
}

char	*handle_cd_home(void)
{
	char	*home;

	home = getenv("HOME");
	if (!home)
	{
		ft_putendl_fd("cd: HOME not set", 2);
		return (NULL);
	}
	return (home);
}

char	*handle_cd_prev(void)
{
	char	*oldpwd;

	oldpwd = getenv("OLDPWD");
	if (!oldpwd)
	{
		ft_putendl_fd("cd: OLDPWD not set", 2);
		return (NULL);
	}
	printf("%s\n", oldpwd);
	return (oldpwd);
}

char	*get_cd_path(char **args)
{
	if (!args[1])
		return (handle_cd_home());
	else if (args[1][0] == '\0')
		return (".");
	else if (ft_strncmp(args[1], "-", 1) == 0 && args[1][1] == '\0')
		return (handle_cd_prev());
	else if (args[1][0] == '~' && (args[1][1] == '\0' || args[1][1] == '/'))
		return (handle_cd_home());
	return (args[1]);
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
