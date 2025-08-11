/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:00:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/11 15:06:59 by tthajan          ###   ########.fr       */
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

void	print_command_error(char *cmd, int error_type)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	if (error_type == 1)
		ft_putstr_fd(": No such file or directory\n", 2);
	else
		ft_putstr_fd(": command not found\n", 2);
}

void	handle_exec_error(char *cmd)
{
	if (ft_strchr(cmd, '/'))
		print_command_error(cmd, 1);
	else
		print_command_error(cmd, 0);
	exit(127);
}

void	print_file_error(char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}
