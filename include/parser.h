/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 18:19:19 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/01 18:22:23 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_cmd
{
	int		is_infile;
	int		append;
	int		here_doc;
	char	*cmd;
	char	**args;
	char	*infile;
	char	*outfile;
	struct s_cmd *next;
}   t_cmd;

#endif
