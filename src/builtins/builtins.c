/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:32:15 by tthajan           #+#    #+#             */
/*   Updated: 2025/08/06 15:23:04 by tthajan          ###   ########.fr       */
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

int	ft_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	
	// Parse -n flag
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

// Helper function to validate variable name
static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	// First character must be letter or underscore
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	// Rest can be alphanumeric or underscore
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(char **args)
{
	int	i;
	char	*equal_sign;
	char	*var_name;
	char	*var_value;
	extern char	**environ;

	i = 1;
	if (!args[1])
	{
		// Print all environment variables (same logic as ft_env)
		i = 0;
		while (environ[i])
		{
			printf("%s\n", environ[i]);
			i++;
		}
		return (0);
	}

	i = 1;
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (!equal_sign)
		{
			// Variable without value - validate name first
			if (!is_valid_identifier(args[i]))
			{
				fprintf(stderr, "export: `%s': not a valid identifier\n", args[i]);
				return (1);
			}
			// Set variable with empty value
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
			
			// Validate variable name
			if (!is_valid_identifier(var_name))
			{
				fprintf(stderr, "export: `%s': not a valid identifier\n", var_name);
				*equal_sign = '='; // restore original string
				return (1);
			}
			
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
	int	exit_status;

	i = 1;
	exit_status = 0;
	
	if (!args[1])
		return (0);
	
	while (args[i])
	{
		// Validate variable name
		if (!is_valid_identifier(args[i]))
		{
			fprintf(stderr, "unset: `%s': not a valid identifier\n", args[i]);
			exit_status = 1;
		}
		else
		{
			// Unset the variable
			if (unsetenv(args[i]) != 0)
			{
				perror("unset");
				exit_status = 1;
			}
		}
		i++;
	}
	return (exit_status);
}

// Helper functions for exit status management without global variables
void	set_last_exit_status(int status)
{
	char	*status_str;
	
	status_str = ft_itoa(status);
	if (status_str)
	{
		setenv("_MINISHELL_EXIT_STATUS", status_str, 1);
		free(status_str);
	}
}

int	get_last_exit_status(void)
{
	char	*status_str;
	int		status;
	
	status_str = getenv("_MINISHELL_EXIT_STATUS");
	if (status_str)
		status = ft_atoi(status_str);
	else
		status = 0; // Default exit status
	return (status);
}

// Expand environment variables in a string
char	*expand_env_vars(char *str)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i;
	int		j;
	int		var_start;
	int		var_len;
	size_t	result_len;

	if (!str)
		return (NULL);
	
	// Calculate required length for result string
	result_len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			if (str[i + 1] == '?')
			{
				// Handle $? - get exit status
				char *exit_status_str = ft_itoa(get_last_exit_status());
				if (exit_status_str)
				{
					result_len += ft_strlen(exit_status_str);
					free(exit_status_str);
				}
				i += 2; // Skip $?
			}
			else if (ft_isalnum(str[i + 1]) || str[i + 1] == '_')
			{
				// Find end of variable name
				var_start = i + 1;
				j = var_start;
				while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
					j++;
				var_len = j - var_start;
			
				// Extract variable name
				var_name = malloc(var_len + 1);
				if (!var_name)
					return (NULL);
				ft_strlcpy(var_name, str + var_start, var_len + 1);
				
				// Get variable value using getenv directly
				var_value = getenv(var_name);
				if (var_value)
					result_len += ft_strlen(var_value);
				free(var_name);
				i = j;
			}
			else
			{
				// Handle literal $ followed by non-variable character
				result_len++;
				i++;
			}
		}
		else
		{
			result_len++;
			i++;
		}
	}
	
	// Allocate result string
	result = malloc(result_len + 1);
	if (!result)
		return (NULL);
	
	// Build result string
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			if (str[i + 1] == '?')
			{
				// Handle $? - get exit status
				char *exit_status_str = ft_itoa(get_last_exit_status());
				if (exit_status_str)
				{
					ft_strlcpy(result + j, exit_status_str, ft_strlen(exit_status_str) + 1);
					j += ft_strlen(exit_status_str);
					free(exit_status_str);
				}
				i += 2; // Skip $?
			}
			else if (ft_isalnum(str[i + 1]) || str[i + 1] == '_')
			{
				// Find end of variable name
				var_start = i + 1;
				var_len = 0;
				while (str[var_start + var_len] && (ft_isalnum(str[var_start + var_len]) || str[var_start + var_len] == '_'))
					var_len++;
			
				// Extract variable name
				var_name = malloc(var_len + 1);
				if (!var_name)
				{
					free(result);
					return (NULL);
				}
				ft_strlcpy(var_name, str + var_start, var_len + 1);
				
				// Get variable value and copy to result using getenv directly
				var_value = getenv(var_name);
				if (var_value)
				{
					ft_strlcpy(result + j, var_value, ft_strlen(var_value) + 1);
					j += ft_strlen(var_value);
				}
				free(var_name);
				i = var_start + var_len;
			}
			else
			{
				// Handle literal $ followed by non-variable character
				result[j++] = str[i++];
			}
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}


