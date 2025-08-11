/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:45:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/11 12:35:11 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_from_list(t_list *env_list, char *key)
{
	t_env	*env_var;

	env_var = find_env_var(env_list, key);
	if (env_var)
		return (env_var->value);
	return (NULL);
}

static char	*extract_var_name(char *str, int *len)
{
	int	i;

	i = 1;
	if (str[i] == '?')
	{
		*len = 2;
		return (ft_strdup("?"));
	}
	if (str[1] == '{')
	{
		i = 2;
		while (str[i] && str[i] != '}' && (ft_isalnum(str[i]) || str[i] == '_'))
			i++;
		if (str[i] == '}')
		{
			*len = i + 1; 
			return (ft_substr(str, 2, i - 2)); 
		}
		*len = 2;
		return (ft_strdup("{"));
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*len = i;
	return (ft_substr(str, 1, i - 1));
}

static char	*get_var_value(char *var_name, t_list *env_list)
{
	char	*value;

	if (ft_strncmp(var_name, "?", 1) == 0)
		return (ft_itoa(get_last_exit_status()));
	
	// Check internal env_list first (if available)
	if (env_list)
	{
		value = get_env_from_list(env_list, var_name);
		if (value)
			return (ft_strdup(value));
	}
		
	// Fall back to system environment
	value = getenv(var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static void	process_var_expansion(char *str, char *result, int *i, int *j, t_list *env_list)
{
	char	*var_name;
	char	*var_value;
	int		var_len;
	int		val_len;

	var_name = extract_var_name(&str[*i], &var_len);
	var_value = get_var_value(var_name, env_list);
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

char	*expand_env_vars(char *str, t_list *env_list)
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
			(ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?' 
				|| str[i + 1] == '{' || str[i + 1] == '}'))
			process_var_expansion(str, result, &i, &j, env_list);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

char	*expand_env_vars_quoted(char *str, char quote_type, t_list *env_list)
{
	if (quote_type == '\'')
		return (ft_strdup(str));
	return (expand_env_vars(str, env_list));
}
