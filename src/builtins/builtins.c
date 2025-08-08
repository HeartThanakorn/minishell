/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:32:15 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/07 18:07:34 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include <errno.h>

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if ((ft_strncmp(cmd, "pwd", 3) == 0 && cmd[3] == '\0')
		|| (ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0')
		|| (ft_strncmp(cmd, "exit", 4) == 0 && cmd[4] == '\0')
		|| (ft_strncmp(cmd, "env", 3) == 0 && cmd[3] == '\0')
		|| (ft_strncmp(cmd, "cd", 2) == 0 && cmd[2] == '\0')
		|| (ft_strncmp(cmd, "export", 6) == 0 && cmd[6] == '\0')
		|| (ft_strncmp(cmd, "unset", 5) == 0 && cmd[5] == '\0'))
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
		return (ft_env(args));
	if (ft_strncmp(args[0], "cd", 2) == 0 && args[0][2] == '\0')
		return (ft_cd(args));
	if (ft_strncmp(args[0], "export", 6) == 0 && args[0][6] == '\0')
		return (ft_export(args));
	if (ft_strncmp(args[0], "unset", 5) == 0 && args[0][5] == '\0')
		return (ft_unset(args));
	return (-1);
}
