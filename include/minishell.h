/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:49:45 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 18:19:21 by tthajan          ###   ########.fr       */
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

// Command structure for parsing
typedef struct s_cmd
{
	int				is_infile;
	int				append;
	int				here_doc;
	char			*delim;
	char			*cmd;
	char			**args;
	char			*infile;
	char			*outfile;
	struct s_cmd	*next;
}	t_cmd;

// Global variable for signal handling - stores only signal number
extern int	g_signal;

// Built-in function declarations
int		is_builtin(char *cmd);
int		execute_builtin(char **args);
int		ft_pwd(void);
int		ft_echo(char **args);
int		ft_exit(char **args);
int		ft_env(char **args);
int		ft_cd(char **args);
int		ft_export(char **args);
int		ft_unset(char **args);

// Command execution
void	exec_cmds(t_cmd *cmd_list);
int		exec_external_cmd(char **args);
char	*resolve_cmd_path(char *cmd);
char	*find_cmd_in_path(char *cmd);
int		execute_child_process(char *cmd_path, char **args);
int		fork_and_execute(char *cmd_path, char **args);

// Signal handling
void	setup_signals(void);
void	setup_child_signals(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

// Redirection functions
int		redirect_input(char *filename);
int		redirect_output(char *filename);
int		redirect_append(char *filename);
int		handle_heredoc(char *delimiter);
int		create_pipe(int *pipefd);

// Pipe functions
void	exec_pipeline(t_cmd *cmd_list);
int		count_commands(t_cmd *cmd_list);
void	exec_single_cmd(t_cmd *cmd, int input_fd, int output_fd);
void	handle_child_process(t_cmd *current, int input_fd, int *pipefd);
void	handle_parent_process(int *input_fd, int *pipefd, t_cmd *current);
void	handle_input_redirection(t_cmd *cmd);
void	handle_output_redirection(t_cmd *cmd);
void	execute_command(t_cmd *cmd);
void	wait_for_children(pid_t *pids, int cmd_count);

// Environment variable access
extern char	**environ;

// Environment variable expansion
char	*expand_env_vars(char *str);
void	set_last_exit_status(int status);
int		get_last_exit_status(void);

// Parser
t_cmd	*parse(t_list *tokens);
char	**list_to_array(t_list *lst);
void	free_array(char **array);
void	free_cmd_lst(t_cmd *cmd);
void	ft_init_cmd(t_cmd *cmd);
void	add_arg(t_list **args_list, char *value);

#endif
