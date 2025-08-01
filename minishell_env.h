/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_env.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:59:54 by kmaeda            #+#    #+#             */
/*   Updated: 2025/07/31 16:19:03 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ENV_H
#define MINISHELL_ENV_H

# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

typedef struct s_shell
{
	t_env	*env_list;
	char	**paths;
}	t_shell;

// Create a new env node from "KEY=VALUE" string
t_env	*create_env_node(char *env_str);

// Initialize linked list from envp
t_env	*init_env_list(char **envp);

// Get value of a key from env list
char	*get_env_value(t_env *env_list, const char *key);

// Parse PATH env variable into array of paths
char	**parse_path(t_env *env_list);

#endif

