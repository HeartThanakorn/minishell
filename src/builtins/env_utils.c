/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 16:27:41 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper functions for exit status management without global variables
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
		status = 0; // Default exit status
	return (status);
}

// Expand environment variables in a string
char	*expand_env_vars(char *str)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i;
	int		j;
	int		var_start;
	int		var_len;
	size_t	result_len;

	if (!str)
		return (NULL);
	
	// Calculate required length for result string
	result_len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			if (str[i + 1] == '?')
			{
				// Handle $? - get exit status
				char *exit_status_str = ft_itoa(get_last_exit_status());
				if (exit_status_str)
				{
					result_len += ft_strlen(exit_status_str);
					free(exit_status_str);
				}
				i += 2; // Skip $?
			}
			else if (ft_isalnum(str[i + 1]) || str[i + 1] == '_')
			{
				// Find end of variable name
				var_start = i + 1;
				j = var_start;
				while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
					j++;
				var_len = j - var_start;
			
				// Extract variable name
				var_name = malloc(var_len + 1);
				if (!var_name)
					return (NULL);
				ft_strlcpy(var_name, str + var_start, var_len + 1);
				
				// Get variable value using getenv directly
				var_value = getenv(var_name);
				if (var_value)
					result_len += ft_strlen(var_value);
				free(var_name);
				i = j;
			}
			else
			{
				// Handle literal $ followed by non-variable character
				result_len++;
				i++;
			}
		}
		else
		{
			result_len++;
			i++;
		}
	}
	
	// Allocate result string
	result = malloc(result_len + 1);
	if (!result)
		return (NULL);
	
	// Build result string
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			if (str[i + 1] == '?')
			{
				// Handle $? - get exit status
				char *exit_status_str = ft_itoa(get_last_exit_status());
				if (exit_status_str)
				{
					ft_strlcpy(result + j, exit_status_str, ft_strlen(exit_status_str) + 1);
					j += ft_strlen(exit_status_str);
					free(exit_status_str);
				}
				i += 2; // Skip $?
			}
			else if (ft_isalnum(str[i + 1]) || str[i + 1] == '_')
			{
				// Find end of variable name
				var_start = i + 1;
				var_len = 0;
				while (str[var_start + var_len] && (ft_isalnum(str[var_start + var_len]) || str[var_start + var_len] == '_'))
					var_len++;
			
				// Extract variable name
				var_name = malloc(var_len + 1);
				if (!var_name)
				{
					free(result);
					return (NULL);
				}
				ft_strlcpy(var_name, str + var_start, var_len + 1);
				
				// Get variable value and copy to result using getenv directly
				var_value = getenv(var_name);
				if (var_value)
				{
					ft_strlcpy(result + j, var_value, ft_strlen(var_value) + 1);
					j += ft_strlen(var_value);
				}
				free(var_name);
				i = var_start + var_len;
			}
			else
			{
				// Handle literal $ followed by non-variable character
				result[j++] = str[i++];
			}
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}
