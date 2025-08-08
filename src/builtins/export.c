/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/07 18:13:27 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	process_export_arg(char **args, int *i)
{
	char	*equal_sign;
	char	*combined_arg;

	equal_sign = ft_strchr(args[*i], '=');
	if (!equal_sign)
		return (export_without_value(args[*i]));
	else if (equal_sign[1] == '\0' && args[*i + 1])
	{
		combined_arg = ft_strjoin(args[*i], args[*i + 1]);
		if (!combined_arg)
			return (1);
		if (export_with_value(combined_arg) != 0)
		{
			free(combined_arg);
			return (1);
		}
		free(combined_arg);
		(*i)++;
		return (0);
	}
	else
		return (export_with_value(args[*i]));
}

int	ft_export(char **args)
{
	int	i;

	if (!args[1])
	{
		print_env_vars();
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args, &i) != 0)
			return (1);
		i++;
	}
	return (0);
}
