/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/08 10:43:10 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char **args)
{
	int		exit_status;
	char	*endptr;

	exit_status = 0;
	printf("exit\n");
	if (args[1])
	{
		exit_status = (int)strtol(args[1], &endptr, 10);
		if (*endptr != '\0' || args[1][0] == '\0')
		{
			fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
			exit_status = 2;
		}
		else if (args[2])
		{
			fprintf(stderr, "exit: too many arguments\n");
			return (1);
		}
	}
	set_last_exit_status(exit_status & 255);
	return (-42);
}
