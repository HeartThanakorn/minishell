/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 10:53:03 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/07 10:26:36 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static t_list	*env_node(char *env_s)
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
	while (envp[i])
	{
		node = env_node(envp[i]);
		if (node)
			ft_lstadd_back(&shell->env_list, node);
		i++;
	}
}
