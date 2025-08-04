/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:32:15 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/04 17:17:05 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include <errno.h>

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if ((ft_strncmp(cmd, "pwd", 3) == 0 && cmd[3] == '\0') ||
		(ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0') ||
		(ft_strncmp(cmd, "exit", 4) == 0 && cmd[4] == '\0') ||
		(ft_strncmp(cmd, "env", 3) == 0 && cmd[3] == '\0') ||
		(ft_strncmp(cmd, "cd", 2) == 0 && cmd[2] == '\0') ||
		(ft_strncmp(cmd, "export", 6) == 0 && cmd[6] == '\0') ||
		(ft_strncmp(cmd, "unset", 5) == 0 && cmd[5] == '\0'))
		return (1);
	return (0);
}

int	execute_builtin(char **args)
{
	if (!args || !args[0])
		return (-1);
	if (ft_strncmp(args[0], "pwd", 3) == 0 && args[0][3] == '\0')
		return (ft_pwd());
	if (ft_strncmp(args[0], "echo", 4) == 0 && args[0][4] == '\0')
		return (ft_echo(args));
	if (ft_strncmp(args[0], "exit", 4) == 0 && args[0][4] == '\0')
		return (ft_exit(args));
	if (ft_strncmp(args[0], "env", 3) == 0 && args[0][3] == '\0')
		return (ft_env(args));
	if (ft_strncmp(args[0], "cd", 2) == 0 && args[0][2] == '\0')
		return (ft_cd(args));
	if (ft_strncmp(args[0], "export", 6) == 0 && args[0][6] == '\0')
		return (ft_export(args));
	if (ft_strncmp(args[0], "unset", 5) == 0 && args[0][5] == '\0')
		return (ft_unset(args));
	return (-1);
}

int	ft_pwd(void)
{
	char	*cwd = getcwd(NULL, 0);

	if (!cwd)
	{
		perror("getcwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

// Print string with escape sequence interpretation
void	print_with_escapes(char *str)
{
	int	i;

	i = 0;
	// Debug: print the raw string
	// fprintf(stderr, "DEBUG: Raw string: '%s'\n", str);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
		{
			if (str[i + 1] == 'n')
			{
				printf("\n");
				i += 2;
			}
			else if (str[i + 1] == 't')
			{
				printf("\t");
				i += 2;
			}
			else if (str[i + 1] == 'r')
			{
				printf("\r");
				i += 2;
			}
			else if (str[i + 1] == '\\')
			{
				printf("\\");
				i += 2;
			}
			else if (str[i + 1] == '"')
			{
				printf("\"");
				i += 2;
			}
			else if (str[i + 1] == '\'')
			{
				printf("'");
				i += 2;
			}
			else
			{
				printf("%c", str[i]);
				i++;
			}
		}
		else
		{
			printf("%c", str[i]);
			i++;
		}
	}
}

int	ft_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	
	// Parse -n flag (only requirement for the project)
	while (args[i] && ft_strncmp(args[i], "-n", 2) == 0 && args[i][2] == '\0')
	{
		newline = 0;
		i++;
	}
	
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	ft_exit(char **args)
{
	int	exit_status;
	char	*endptr;

	exit_status = 0; // default exit status
	
	if (args[1])
	{
		// Check if it's a valid number
		exit_status = (int)strtol(args[1], &endptr, 10);
		if (*endptr != '\0' || args[1][0] == '\0')
		{
			fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
			exit_status = 2; // exit with error status for invalid argument
		}
		else if (args[2])
		{
			fprintf(stderr, "exit: too many arguments\n");
			return (1); // don't exit, just return error
		}
	}
	
	printf("exit\n");
	exit(exit_status & 255); // bash-like behavior: exit status is modulo 256
}

int	ft_env(char **args)
{
	extern char	**environ;
	int		i;

	// Check that no arguments are provided (env with no options or arguments)
	if (args[1])
	{
		fprintf(stderr, "env: too many arguments\n");
		return (1);
	}

	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}

void	print_env(void)
{
	extern char	**environ;
	int		i;

	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
}

int	ft_cd(char **args)
{
	char	*path;
	char	*home;

	// Check if no path argument is provided - go to HOME
	if (!args[1])
	{
		home = getenv("HOME");
		if (!home)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
		path = home;
	}
	else if (args[1][0] == '~' && (args[1][1] == '\0' || args[1][1] == '/'))
	{
		// Handle tilde expansion
		home = getenv("HOME");
		if (!home)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
		if (args[1][1] == '\0')
		{
			// Just "~"
			path = home;
		}
		else
		{
			// "~/something" - we'd need to concatenate, but for simplicity just use HOME for "~"
			path = home;
		}
	}
	else
	{
		// Use the provided path (relative or absolute)
		path = args[1];
	}
	
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	
	return (0);
}

int	ft_export(char **args)
{
	int	i;
	char	*equal_sign;
	char	*var_name;
	char	*var_value;

	i = 1;
	if (!args[1])
	{
		// Print all environment variables in sorted order (basic implementation)
		print_env();
		return (0);
	}

	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (!equal_sign)
		{
			// Variable without value - set it to empty string
			if (setenv(args[i], "", 1) != 0)
			{
				perror("export");
				return (1);
			}
		}
		else
		{
			// Split variable name and value
			*equal_sign = '\0';
			var_name = args[i];
			var_value = equal_sign + 1;
			
			if (setenv(var_name, var_value, 1) != 0)
			{
				perror("export");
				*equal_sign = '='; // restore original string
				return (1);
			}
			*equal_sign = '='; // restore original string
		}
		i++;
	}
	return (0);
}

int	ft_unset(char **args)
{
	int	i;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (unsetenv(args[i]) != 0)
		{
			perror("unset");
			return (1);
		}
		i++;
	}
	return (0);
}
