/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 17:00:46 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(char *str)
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

static int	export_without_value(char *arg)
{
	if (!is_valid_identifier(arg))
	{
		fprintf(stderr, "export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	if (setenv(arg, "", 1) != 0)
	{
		perror("export");
		return (1);
	}
	return (0);
}

static int	export_with_value(char *arg)
{
	char	*equal_sign;
	char	*var_name;
	char	*var_value;

	equal_sign = ft_strchr(arg, '=');
	*equal_sign = '\0';
	var_name = arg;
	var_value = equal_sign + 1;
	if (!is_valid_identifier(var_name))
	{
		fprintf(stderr, "export: `%s': not a valid identifier\n", var_name);
		*equal_sign = '=';
		return (1);
	}
	if (setenv(var_name, var_value, 1) != 0)
	{
		perror("export");
		*equal_sign = '=';
		return (1);
	}
	*equal_sign = '=';
	return (0);
}

static void	print_env_vars(void)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
}

int	ft_export(char **args)
{
	int		i;
	char	*equal_sign;

	if (!args[1])
	{
		print_env_vars();
		return (0);
	}
	i = 1;
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (!equal_sign)
		{
			if (export_without_value(args[i]) != 0)
				return (1);
		}
		else
		{
			if (export_with_value(args[i]) != 0)
				return (1);
		}
		i++;
	}
	return (0);
}
