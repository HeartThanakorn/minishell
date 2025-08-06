/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 16:27:41 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to validate variable name
static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	// First character must be letter or underscore
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	// Rest can be alphanumeric or underscore
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(char **args)
{
	int	i;
	char	*equal_sign;
	char	*var_name;
	char	*var_value;
	extern char	**environ;

	i = 1;
	if (!args[1])
	{
		// Print all environment variables (same logic as ft_env)
		i = 0;
		while (environ[i])
		{
			printf("%s\n", environ[i]);
			i++;
		}
		return (0);
	}

	i = 1;
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (!equal_sign)
		{
			// Variable without value - validate name first
			if (!is_valid_identifier(args[i]))
			{
				fprintf(stderr, "export: `%s': not a valid identifier\n", args[i]);
				return (1);
			}
			// Set variable with empty value
			if (setenv(args[i], "", 1) != 0)
			{
				perror("export");
				return (1);
			}
		}
		else
		{
			// Split variable name and value
			*equal_sign = '\0';
			var_name = args[i];
			var_value = equal_sign + 1;
			
			// Validate variable name
			if (!is_valid_identifier(var_name))
			{
				fprintf(stderr, "export: `%s': not a valid identifier\n", var_name);
				*equal_sign = '='; // restore original string
				return (1);
			}
			
			if (setenv(var_name, var_value, 1) != 0)
			{
				perror("export");
				*equal_sign = '='; // restore original string
				return (1);
			}
			*equal_sign = '='; // restore original string
		}
		i++;
	}
	return (0);
}
