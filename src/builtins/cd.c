/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 18:45:06 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_cd_home(void)
{
	char	*home;

	home = getenv("HOME");
	if (!home)
	{
		fprintf(stderr, "cd: HOME not set\n");
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
		fprintf(stderr, "cd: OLDPWD not set\n");
		return (NULL);
	}
	printf("%s\n", oldpwd);
	return (oldpwd);
}

static char	*get_cd_path(char **args)
{
	if (!args[1])
		return (handle_cd_home());
	else if (ft_strncmp(args[1], "-", 1) == 0 && args[1][1] == '\0')
		return (handle_cd_prev());
	else if (args[1][0] == '~' && (args[1][1] == '\0' || args[1][1] == '/'))
		return (handle_cd_home());
	return (args[1]);
}

static void	update_pwd_vars(char *old_pwd)
{
	char	*new_pwd;

	if (old_pwd)
	{
		setenv("OLDPWD", old_pwd, 1);
		free(old_pwd);
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		setenv("PWD", new_pwd, 1);
		free(new_pwd);
	}
}

int	ft_cd(char **args)
{
	char	*path;
	char	*pwd;

	pwd = getcwd(NULL, 0);
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
	update_pwd_vars(pwd);
	return (0);
}
