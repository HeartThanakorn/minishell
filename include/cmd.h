/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 10:20:44 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/07 17:58:05 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include "libft.h"

typedef struct s_shell	t_shell;

typedef struct s_cmd
{
	int				is_infile;
	int				append;
	int				here_doc;
	int				fd[2];
	int				prev_fd;
	int				in_fd;
	int				out_fd;
	char			*path;
	char			*delim;
	char			*cmd;
	char			**args;
	char			*infile;
	char			*outfile;
	struct s_cmd	*next;
}	t_cmd;

t_cmd	*parse(t_list *tokens);
char	**list_to_array(t_list *lst);
void	free_array(char **array);
void	free_cmd_lst(t_cmd *cmd);
void	ft_init_cmd(t_cmd *cmd);
void	add_arg(t_list **args_list, char *value, char quote_type);
int		init_files(t_cmd *cmd);
void	exec_cmd(t_cmd *cmd, t_shell *shell, char **envp);
void	exec_pipe(t_cmd *cmd_list, int cmd_count, char **envp);
int		count_commands(t_cmd *cmd_list);

#endif
