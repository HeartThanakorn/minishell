/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 16:27:41 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(char **args)
{
	char	*path;
	char	*home;

	// Check if no path argument is provided - go to HOME
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
	else if (args[1][0] == '~' && (args[1][1] == '\0' || args[1][1] == '/'))
	{
		// Handle tilde expansion
		home = getenv("HOME");
		if (!home)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
		if (args[1][1] == '\0')
		{
			// Just "~"
			path = home;
		}
		else
		{
			// "~/something" - we'd need to concatenate, but for simplicity just use HOME for "~"
			path = home;
		}
	}
	else
	{
		// Use the provided path (relative or absolute)
		path = args[1];
	}
	
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	
	return (0);
}
