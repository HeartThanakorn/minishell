/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/07 18:07:34 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset_variable(char *var_name)
{
	if (!is_valid_identifier(var_name))
	{
		fprintf(stderr, "unset: `%s': not a valid identifier\n", var_name);
		return (1);
	}
	if (unsetenv(var_name) != 0)
	{
		perror("unset");
		return (1);
	}
	return (0);
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
		if (unset_variable(args[i]) != 0)
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
