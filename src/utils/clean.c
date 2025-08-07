/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 10:40:25 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/07 10:40:47 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "token.h"
#include "cmd.h"
#include <stdlib.h>
#include <errno.h>

void	free_env(void *content)
{
	t_env	*env;

	env = (t_env *)content;
	if (env->key)
		free(env->key);
	if (env->value)
		free(env->value);
	free(env);
}

void	free_cmd_lst(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		free(cmd->cmd);
		free_array(cmd->args);
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		if (cmd->delim)
			free(cmd->delim);
		if (cmd->fd[0] > 0)
			close(cmd->fd[0]);
		if (cmd->fd[1] > 0)
			close(cmd->fd[1]);
		free(cmd);
		cmd = next;
	}
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	ft_clean(t_cmd *cmd, t_shell *shell)
{
	int	i;

	i = 0;
	if (cmd->in_fd >= 0)
		close(cmd->in_fd);
	if (cmd->out_fd >= 0)
		close(cmd->out_fd);
	if (cmd->args)
		free_array(cmd->args);
	if (cmd->path)
		free(cmd->path);
	if (shell->paths)
		free_array(shell->paths);
	if (shell->env_list)
		ft_lstclear(&shell->env_list, free_env);
	if (cmd)
		free_cmd_lst(cmd);
}

void	error_exit(t_shell *shell, t_cmd *cmd, char *msg)
{
	perror(msg);
	ft_clean(cmd, shell);
	exit(1);
}
