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
# define MINISHELL_ENV_H

# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

typedef struct s_shell
{
	t_list	*env_list;
	char	**paths;
}	t_shell;

typedef struct s_cmd
{
	int				is_infile;
	int				append;
	int				here_doc;
	char			*delim;
	char			*cmd;
	char			**args;
	char			*infile;
	char			*outfile;
	struct s_cmd	*next;
}	t_cmd;

t_list	*env_node(char *env_s);
void	init_env(t_shell *shell, char **envp);
char	**get_path(t_list *env_list);
char	*get_path_s(t_list *env_list);
t_cmd	*parse(t_list *tokens);
char	**list_to_array(t_list *lst);
void	free_array(char **array);
void	free_cmd_lst(t_cmd *cmd);
void	ft_init_cmd(t_cmd *cmd);
void	add_arg(t_list **args_list, char *value);

#endif
