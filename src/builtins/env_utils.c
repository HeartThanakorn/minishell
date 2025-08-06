/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 17:00:46 by tthajan          ###   ########.fr       */
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

char	*expand_env_vars(char *str)
{
	char	*result;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) * 2 + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			result[j++] = '0';
			i += 2;
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}
