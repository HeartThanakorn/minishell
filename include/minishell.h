/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:49:45 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 11:30:40 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include "get_next_line.h"
#include "minishell_env.h"

// Global variable for signal handling - stores only signal number
extern int	g_signal;

// Built-in function declarations
int	is_builtin(char *cmd);
int	execute_builtin(char **args);
int	ft_pwd(void);
int	ft_echo(char **args);
int	ft_exit(char **args);
int	ft_env(char **args);
int	ft_cd(char **args);
int	ft_export(char **args);
int	ft_unset(char **args);

// Command execution
void	exec_cmds(t_cmd *cmd_list, t_shell *shell);
int		exec_external_cmd(char **args, char **paths);

// Signal handling
void	setup_signals(void);
void	setup_child_signals(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

// Redirection functions
int	redirect_input(char *filename);
int	redirect_output(char *filename);
int	redirect_append(char *filename);
int	handle_heredoc(char *delimiter);
int	create_pipe(int *pipefd);

// Environment variable access
extern char	**environ;

#endif
