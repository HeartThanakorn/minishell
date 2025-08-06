/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:30:00 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 18:45:06 by tthajan          ###   ########.fr       */
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
