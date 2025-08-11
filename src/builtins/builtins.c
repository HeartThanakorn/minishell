/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:32:15 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/11 12:35:11 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	strcmp_exact(const char *s1, const char *s2)
{
	return (ft_strncmp(s1, s2, ft_strlen(s2)) == 0 && s1[ft_strlen(s2)] == '\0');
}
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
	if (strcmp_exact(cmd, "pwd") || strcmp_exact(cmd, "echo") 
		|| strcmp_exact(cmd, "exit") || strcmp_exact(cmd, "env")
		|| strcmp_exact(cmd, "cd") || strcmp_exact(cmd, "export")
		|| strcmp_exact(cmd, "unset"))
		return (1);
	return (0);
}

int	execute_builtin(char **args, t_shell *shell)
{
	if (!args || !args[0])
		return (-1);
	if (strcmp_exact(args[0], "pwd"))
		return (ft_pwd());
	if (strcmp_exact(args[0], "echo"))
		return (ft_echo(args));
	if (strcmp_exact(args[0], "exit"))
		return (ft_exit(args));
	if (strcmp_exact(args[0], "env"))
		return (ft_env(args, shell));
	if (strcmp_exact(args[0], "cd"))
		return (ft_cd(args, shell));
	if (strcmp_exact(args[0], "export"))
		return (ft_export(args, shell));
	if (strcmp_exact(args[0], "unset"))
		return (ft_unset(args, shell));
	return (-1);
}
