/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:49:45 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/11 14:31:53 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "get_next_line.h"
# include "token.h"
# include "cmd.h"
# include "env.h"

// Global variable for signal handling
extern int	g_signal;

// Built-in function declarations
int		is_builtin(char *cmd);
int		execute_builtin(char **args, t_shell *shell);
int		is_valid_identifier(char *str);
void	print_invalid_identifier_error(char *command, char *identifier);
t_env	*find_env_var(t_list *env_list, char *key);
int		ft_pwd(t_shell *shell);
int		ft_echo(char **args);
int		ft_exit(char **args);
int		ft_env(char **args, t_shell *shell);
int		ft_cd(char **args, t_shell *shell);
int		ft_export(char **args, t_shell *shell);
int		ft_unset(char **args, t_shell *shell);

// Signal handling
void	setup_signals(void);
void	setup_child_signals(void);
void	handle_sigint(int sig);

// Utility functions
int		get_last_exit_status(void);
void	set_last_exit_status(int status);
char	*expand_env_vars(char *str, t_list *env_list);
char	*expand_env_vars_quoted(char *str, char quote_type, t_list *env_list);
void	error_exit(t_shell *shell, t_cmd *cmd, char *msg);
void	ft_clean(t_cmd *cmd, t_shell *shell);

// Redirection functions
int		redirect_input(char *filename);
int		redirect_output(char *filename);
int		redirect_append(char *filename);
int		handle_heredoc(char *delimiter, t_shell *shell);

// Pipe utilities
void	execute_command(t_cmd *cmd, t_shell *shell, char **envp);
void	handle_pipe_signal_exit(int status);
void	handle_signal_exit(int status);
void	wait_for_pipes(pid_t *pids, int cmd_count);

// Exec utilities
int		init_input_files(t_cmd *cmd, t_shell *shell);
int		init_output_files(t_cmd *cmd);
void	exec_child_command(t_cmd *cmd, t_shell *shell, char **envp);

// Parser helper functions
void	set_redir_flags(t_cmd *cmd, int redir_type);
void	assign_redir_file(t_cmd *cmd, int redir_type, char *filename);
void	process_word_tokens(t_list **tokens, t_list **args_list, t_shell *shell);
int		handle_token(t_list **tokens, t_cmd *cmd, t_shell *shell);
int		process_cmd_tokens(t_list **tokens, t_cmd *cmd, t_shell *shell);
int		parse_redir(t_list **tokens, t_cmd *cmd, t_shell *shell);
void	parse_args(t_list **tokens, t_cmd *cmd, t_shell *shell);
int		count_commands(t_cmd *cmd_list);


#endif
