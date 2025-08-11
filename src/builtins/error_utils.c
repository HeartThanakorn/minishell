/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:00:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/11 12:35:11 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_invalid_identifier_error(char *command, char *identifier)
{
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(identifier, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

t_env	*find_env_var(t_list *env_list, char *key)
{
	t_list	*current;
	t_env	*env_var;

	if (!env_list || !key)
		return (NULL);
	current = env_list;
	while (current)
	{
		env_var = (t_env *)current->content;
		if (ft_strncmp(env_var->key, key, ft_strlen(key)) == 0 
			&& ft_strlen(env_var->key) == ft_strlen(key))
			return (env_var);
		current = current->next;
	}
	return (NULL);
}
