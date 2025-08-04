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

typedef struct s_command
{
    char    *cmd;           // command name (e.g. "ls", "echo")
    char    **args;         // NULL-terminated array of arguments (including cmd at args[0])
    char    *input_file;    // filename for input redirection (<)
    char    *output_file;   // filename for output redirection (>)
    int     output_append;  // flag for append mode (>>) vs overwrite (>)
    struct s_command *next; // pointer to next command in pipeline (for '|')
}   t_command;

