/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:45:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/07 10:42:06 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_var_name(char *str, int *len)
{
	int	i;

	i = 1;
	if (str[i] == '?')
	{
		*len = 2;
		return (ft_strdup("?"));
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*len = i;
	return (ft_substr(str, 1, i - 1));
}

static char	*get_var_value(char *var_name)
{
	char	*value;

	if (ft_strncmp(var_name, "?", 1) == 0)
		return (ft_itoa(get_last_exit_status()));
	value = getenv(var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static void	process_var_expansion(char *str, char *result, int *i, int *j)
{
	char	*var_name;
	char	*var_value;
	int		var_len;
	int		val_len;

	var_name = extract_var_name(&str[*i], &var_len);
	var_value = get_var_value(var_name);
	if (var_value)
	{
		val_len = ft_strlen(var_value);
		ft_memcpy(&result[*j], var_value, val_len);
		*j += val_len;
		free(var_value);
	}
	free(var_name);
	*i += var_len;
}

char	*expand_env_vars(char *str)
{
	char	*result;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) * 8 + 1024);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && 
			(ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
		{
			process_var_expansion(str, result, &i, &j);
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}
