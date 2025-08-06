/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 16:27:41 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **args)
{
	extern char	**environ;
	int		i;

	// Check that no arguments are provided (env with no options or arguments)
	if (args[1])
	{
		fprintf(stderr, "env: too many arguments\n");
		return (1);
	}

	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}
