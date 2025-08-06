/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
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

int	ft_unset(char **args)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	
	if (!args[1])
		return (0);
	
	while (args[i])
	{
		// Validate variable name
		if (!is_valid_identifier(args[i]))
		{
			fprintf(stderr, "unset: `%s': not a valid identifier\n", args[i]);
			exit_status = 1;
		}
		else
		{
			// Unset the variable
			if (unsetenv(args[i]) != 0)
			{
				perror("unset");
				exit_status = 1;
			}
		}
		i++;
	}
	return (exit_status);
}
