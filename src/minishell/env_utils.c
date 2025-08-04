/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 10:53:03 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/04 18:27:11 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_env.h"
#include <stdlib.h>

t_list	*env_node(char *env_s)
{
	int		key_len;
	char	*equal;
	t_env	*var;
	t_list	*node;

	equal = ft_strchr(env_s, '=');
	if (!equal)
		return (NULL);
	key_len = equal - env_s;
	var = malloc(sizeof(t_env));
	if (!var)
		return (NULL);
	var->key = ft_substr(env_s, 0, key_len);
	var->value = ft_strdup(equal + 1);
	node = ft_lstnew(var);
	return (node);
}

void	init_env(t_shell *shell, char **envp)
{
	int		i;
	t_list	*node;

	i = 0;
	shell->env_list = NULL;
	shell->last_exit_status = 0;  // Initialize exit status to 0
	while (envp[i])
	{
		node = env_node(envp[i]);
		if (node)
			ft_lstadd_back(&shell->env_list, node);
		i++;
	}
}

void	free_env_node(void *content)
{
	t_env	*env;

	if (!content)
		return ;
	env = (t_env *)content;
	if (env->key)
		free(env->key);
	if (env->value)
		free(env->value);
	free(env);
}

void	cleanup_shell(t_shell *shell)
{
	int	i;

	if (shell->env_list)
	{
		ft_lstclear(&shell->env_list, free_env_node);
		shell->env_list = NULL;
	}
	if (shell->paths)
	{
		i = 0;
		while (shell->paths[i])
		{
			free(shell->paths[i]);
			i++;
		}
		free(shell->paths);
		shell->paths = NULL;
	}
}
