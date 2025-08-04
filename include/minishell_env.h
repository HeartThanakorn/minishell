/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_env.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:59:54 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/04 18:27:11 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ENV_H
# define MINISHELL_ENV_H

# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

typedef struct s_shell
{
	t_list	*env_list;
	char	**paths;
	int		last_exit_status;
}	t_shell;

// Create a new env node from "KEY=VALUE" string
t_env	*create_env_node(char *env_str);

// Initialize linked list from envp
t_env	*init_env_list(char **envp);

// Get value of a key from env list
char	*get_env_value(t_list *env_list, const char *key);

// Parse PATH env variable into array of paths
char	**parse_path(t_list *env_list);

// Missing function declarations that are used in minishell.c
void	init_env(t_shell *shell, char **envp);
char	**get_path(t_list *env_list);

// Cleanup functions
void	free_env_node(void *content);
void	cleanup_shell(t_shell *shell);

// External global signal variable
extern int	g_signal;

#endif
