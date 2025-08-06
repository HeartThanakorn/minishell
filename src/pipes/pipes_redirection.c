/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:30:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 17:12:48 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input_redirection(t_cmd *cmd)
{
	if (cmd->here_doc)
	{
		if (handle_heredoc(cmd->delim) == -1)
			return ;
	}
	else if (cmd->is_infile && cmd->infile)
	{
		if (redirect_input(cmd->infile) == -1)
			return ;
	}
}

void	handle_output_redirection(t_cmd *cmd)
{
	if (cmd->outfile)
	{
		if (cmd->append)
		{
			if (redirect_append(cmd->outfile) == -1)
				return ;
		}
		else
		{
			if (redirect_output(cmd->outfile) == -1)
				return ;
		}
	}
}

void	execute_command(t_cmd *cmd)
{
	int	exit_status;

	if (is_builtin(cmd->cmd))
	{
		exit_status = execute_builtin(cmd->args);
		set_last_exit_status(exit_status);
	}
	else
	{
		exit_status = exec_external_cmd(cmd->args);
		if (exit_status == -1)
		{
			ft_printf("minishell: %s: command not found\n", cmd->cmd);
			set_last_exit_status(127);
		}
		else
		{
			set_last_exit_status(exit_status);
		}
	}
}

void	exec_single_cmd(t_cmd *cmd, int input_fd, int output_fd)
{
	(void)input_fd;
	(void)output_fd;
	if (!cmd || !cmd->cmd)
		return ;
	handle_input_redirection(cmd);
	handle_output_redirection(cmd);
	execute_command(cmd);
}
