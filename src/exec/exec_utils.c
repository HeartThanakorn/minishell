/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:00:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/11 13:15:37 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_input_files(t_cmd *cmd, t_shell *shell)
{
	if (cmd->here_doc && cmd->delim)
	{
		cmd->in_fd = handle_heredoc(cmd->delim, shell);
		if (cmd->in_fd < 0)
			return (ft_putstr_fd("Error: could not handle heredoc\n", 2), 1);
	}
	else if (cmd->infile)
	{
		cmd->in_fd = open(cmd->infile, O_RDONLY);
		if (cmd->in_fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->infile, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (1);
		}
	}
	return (0);
}

int	init_output_files(t_cmd *cmd)
{
	if (cmd->outfile)
	{
		if (cmd->append)
			cmd->out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND,
					0644);
		else
			cmd->out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC,
					0644);
		if (cmd->out_fd < 0)
		{
			if (cmd->in_fd != -1)
				close(cmd->in_fd);
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->outfile, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (1);
		}
	}
	return (0);
}

void	exec_child_command(t_cmd *cmd, t_shell *shell, char **envp)
{
	int	exit_code;

	if (is_builtin(cmd->cmd))
	{
		exit_code = execute_builtin(cmd->args, shell);
		if (exit_code == -42)
			exit(get_last_exit_status());
		exit(exit_code);
	}
	if (!cmd->path)
	{
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (execve(cmd->path, cmd->args, envp) == -1)
	{
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
}
