/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:32:15 by tthajan           #+#    #+#             */
/*   Updated: 2025/07/31 17:49:01 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if ((ft_strncmp(cmd, "pwd", 3) == 0 && cmd[3] == '\0') ||
		(ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0') ||
		(ft_strncmp(cmd, "exit", 4) == 0 && cmd[4] == '\0') ||
		(ft_strncmp(cmd, "env", 3) == 0 && cmd[3] == '\0') ||
		(ft_strncmp(cmd, "cd", 2) == 0 && cmd[2] == '\0') ||
		(ft_strncmp(cmd, "export", 6) == 0 && cmd[6] == '\0') ||
		(ft_strncmp(cmd, "unset", 5) == 0 && cmd[5] == '\0'))
		return (1);
	return (0);
}

int	execute_builtin(char **args)
{
	if (!args || !args[0])
		return (-1);
	if (ft_strncmp(args[0], "pwd", 3) == 0 && args[0][3] == '\0')
		return (ft_pwd());
	if (ft_strncmp(args[0], "echo", 4) == 0 && args[0][4] == '\0')
		return (ft_echo(args));
	if (ft_strncmp(args[0], "exit", 4) == 0 && args[0][4] == '\0')
		return (ft_exit(args));
	if (ft_strncmp(args[0], "env", 3) == 0 && args[0][3] == '\0')
		return (ft_env());
	if (ft_strncmp(args[0], "cd", 2) == 0 && args[0][2] == '\0')
		return (ft_cd(args));
	if (ft_strncmp(args[0], "export", 6) == 0 && args[0][6] == '\0')
		return (ft_export(args));
	if (ft_strncmp(args[0], "unset", 5) == 0 && args[0][5] == '\0')
		return (ft_unset(args));
	return (-1);
}

int	ft_pwd(void)
{
	char	*cwd = getcwd(NULL, 0);

	if (!cwd)
	{
		perror("getcwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	ft_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0 && args[1][2] == '\0')
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	ft_exit(char **args)
{
	(void)args;
	printf("exit\n");
	exit(0);
}

int	ft_env(void)
{
	extern char	**environ;
	int		i;

	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}

int	ft_cd(char **args)
{
	char	*path;
	char	*home;

	if (!args[1])
	{
		home = getenv("HOME");
		if (!home)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
		path = home;
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	ft_export(char **args)
{
	int	i;
	char	*equal_sign;

	i = 1;
	if (!args[1])
	{
		// Print all environment variables in sorted order (basic implementation)
		ft_env();
		return (0);
	}

	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (!equal_sign)
		{
			// Variable without value - just skip for now
			// In a full implementation, you'd set it to empty string
			i++;
			continue ;
		}
		if (putenv(args[i]) != 0)
		{
			perror("export");
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_unset(char **args)
{
	int	i;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (unsetenv(args[i]) != 0)
		{
			perror("unset");
			return (1);
		}
		i++;
	}
	return (0);
}
