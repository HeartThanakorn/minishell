/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/11 12:35:11 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static void	set_env_value(t_list *env_list, char *key, char *value)
{
	t_env	*env_var;

	env_var = find_env_var(env_list, key);
	if (env_var)
	{
		free(env_var->value);
		env_var->value = ft_strdup(value);
	}
}

static char	*handle_cd_home(void)
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

static char	*handle_cd_prev(void)
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

static char	*get_cd_path(char **args)
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

static void	update_pwd_vars(char *old_pwd, char *new_logical_path, t_shell *shell)
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

int	ft_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*pwd;
	char	*new_logical_path;

	pwd = getenv("PWD");
	if (pwd)
		pwd = ft_strdup(pwd);
	path = get_cd_path(args);
	if (!path)
	{
		free(pwd);
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		free(pwd);
		return (1);
	}
	if (path[0] == '/')
		new_logical_path = ft_strdup(path);
	else
		new_logical_path = getcwd(NULL, 0);
	update_pwd_vars(pwd, new_logical_path, shell);
	free(new_logical_path);
	return (0);
}
