/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 10:17:32 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/07 17:58:05 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "libft.h"

// Forward declaration
typedef struct s_cmd	t_cmd;

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

typedef struct s_shell
{
	int		cmd_count;
	int		should_exit;
	int		exit_status;
	t_list	*env_list;
	char	**paths;
}	t_shell;

void	init_env(t_shell *shell, char **envp);
char	**get_path(t_list *env_list);
int		cmd_path(t_cmd *cmd_list, char **paths);

#endif
