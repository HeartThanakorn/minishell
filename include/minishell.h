/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:49:45 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/04 18:27:11 by tthajan          ###   ########.fr       */
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
#include "../libft/libft.h"
#include "../libft/get_next_line/get_next_line.h"

// Global variable for signal handling
extern int	g_signal;

// Forward declarations
typedef struct s_shell t_shell;

// Pipeline structure for handling multiple commands connected by pipes
typedef struct s_pipeline
{
	char	***commands;	// Array of command arrays
	int		cmd_count;		// Number of commands in pipeline
	int		**pipes;		// Array of pipe file descriptors
	pid_t	*pids;			// Array of process IDs
}	t_pipeline;

// Built-in function declarations
int	is_builtin(char *cmd);
int	execute_builtin(char **args);
int	ft_pwd(void);
int	ft_echo(char **args);
int	ft_exit(char **args);
int	ft_env(char **args);
void	print_env(void);
int	ft_cd(char **args);
int	ft_export(char **args);
int	ft_unset(char **args);

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

// Command execution with redirections
char	**process_redirections_and_args(t_list *tokens, t_shell *shell);

// Pipeline functions
t_pipeline	*create_pipeline(t_list *tokens, t_shell *shell);
void		free_pipeline(t_pipeline *pipeline);
int			execute_pipeline(t_pipeline *pipeline, t_shell *shell);
t_list		**split_tokens_by_pipes(t_list *tokens, int *cmd_count);
int			has_pipes(t_list *tokens);

// Environment variable access
extern char	**environ;

#endif
