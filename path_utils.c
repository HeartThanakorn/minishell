/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 10:53:40 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/01 10:54:16 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_env.h"

char	*get_path_s(t_list *env_list)
{
	t_env	*env;

	while (env_list)
	{
		env = (t_env *)env_list->content;
		if (ft_strncmp(env->key, "PATH", 4) == 0)
			return (env->value);
		env_list = env_list->next;
	}
	return (NULL);
}

char	**get_path(t_list *env_list)
{
	char	*path;

	path = get_path_s(env_list);
	if (!path)
		return (NULL);
	return (ft_split(path, ':'));
}
