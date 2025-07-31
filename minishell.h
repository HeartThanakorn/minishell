/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:49:45 by tthajan           #+#    #+#             */
/*   Updated: 2025/07/31 17:50:38 by tthajan          ###   ########.fr       */
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
#include "libft/libft.h"
#include "libft/get_next_line/get_next_line.h"

// Global variable for signal handling
extern int	g_signal;

// Built-in function declarations
int	is_builtin(char *cmd);
int	execute_builtin(char **args);
int	ft_pwd(void);
int	ft_echo(char **args);
int	ft_exit(char **args);
int	ft_env(void);
int	ft_cd(char **args);
int	ft_export(char **args);
int	ft_unset(char **args);

// Signal handling
void	setup_signals(void);
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
