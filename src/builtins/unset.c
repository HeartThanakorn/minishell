/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:20:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/11 15:05:57 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset_variable(char *var_name, t_shell *shell)
{
	t_list	*current;
	t_list	*prev;
	t_env	*env_var;

	if (!is_valid_identifier(var_name))
	{
		print_invalid_identifier_error("unset", var_name);
		return (1);
	}
	current = shell->env_list;
	prev = NULL;
	while (current)
	{
		env_var = (t_env *)current->content;
		if (ft_strncmp(env_var->key, var_name, ft_strlen(var_name)) == 0 
			&& ft_strlen(env_var->key) == ft_strlen(var_name))
		{
			if (prev)
				prev->next = current->next;
			else
				shell->env_list = current->next;
			free(env_var->key);
			free(env_var->value);
			free(env_var);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

int	ft_unset(char **args, t_shell *shell)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (unset_variable(args[i], shell) != 0)
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
