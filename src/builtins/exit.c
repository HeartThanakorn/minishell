/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 16:27:41 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char **args)
{
	int	exit_status;
	char	*endptr;

	exit_status = 0; // default exit status
	
	if (args[1])
	{
		// Check if it's a valid number
		exit_status = (int)strtol(args[1], &endptr, 10);
		if (*endptr != '\0' || args[1][0] == '\0')
		{
			fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
			exit_status = 2; // exit with error status for invalid argument
		}
		else if (args[2])
		{
			fprintf(stderr, "exit: too many arguments\n");
			return (1); // don't exit, just return error
		}
	}
	
	printf("exit\n");
	exit(exit_status & 255); // bash-like behavior: exit status is modulo 256
}
