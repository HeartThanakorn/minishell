/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 10:17:32 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/07 10:20:34 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "libft.h"

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

typedef struct s_shell
{
	int		cmd_count;
	t_list	*env_list;
	char	**paths;
}	t_shell;

void	init_env(t_shell *shell, char **envp);
char	**get_path(t_list *env_list);
int		cmd_path(t_cmd *cmd_list, char **paths);

#endif
