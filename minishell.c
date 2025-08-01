/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:19:07 by kmaeda            #+#    #+#             */
/*   Updated: 2025/07/31 13:41:40 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_env.h"

/*void	print_env(t_list *env_list)
{
	t_env	*env;

	while (env_list)
	{
		env = (t_env *)env_list->content;
		printf("%s=%s\n", env->key, env->value);
		env_list = env_list->next;
	}
}*/

t_list	*env_node(char *env_s)
{
	int	key_len;
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
	int	i;
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

void	minishell_loop(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell shell;

	(void)argc;
	(void)argv;
	init_env(&shell, envp);
	shell.paths = get_path(shell.env_list);
	minishell_loop();
	return (0);
}
