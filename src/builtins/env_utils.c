/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 18:45:06 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_last_exit_status(int status)
{
	char	*status_str;

	status_str = ft_itoa(status);
	if (status_str)
	{
		setenv("_MINISHELL_EXIT_STATUS", status_str, 1);
		free(status_str);
	}
}

int	get_last_exit_status(void)
{
	char	*status_str;
	int		status;

	status_str = getenv("_MINISHELL_EXIT_STATUS");
	if (status_str)
		status = ft_atoi(status_str);
	else
		status = 0;
	return (status);
}
