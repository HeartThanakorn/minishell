/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:44:55 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/04 17:56:59 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "minishell_env.h"

void	add_token(t_list **tokens, char *s)
{
	t_list	*new_node;

	new_node = ft_lstnew(s);
	if (!new_node)
		return ;
	ft_lstadd_back(tokens, new_node);
}

int	token_size(char *s, char quote)
{
	int	size;

	size = 0;
	while (s[size] && s[size] != quote)
		size++;
	return (size);
}

void	str_quote(t_list **tokens, char **temp)
{
	int		i;
	int		size;
	char	quote;
	char	*s;
	char	*t;
	char	*final_token;

	i = 0;
	t = *temp;
	quote = *t++;
	size = token_size(t, quote);
	s = malloc(size + 1);
	if (!s)
		return ;
	while (i < size)
	{
		s[i] = t[i];
		i++;
	}
	s[size] = '\0';
	
	// For single quotes, add a special prefix to prevent expansion
	if (quote == '\'')
	{
		final_token = malloc(size + 2); // +1 for prefix, +1 for null terminator
		if (!final_token)
		{
			free(s);
			return ;
		}
		final_token[0] = '\x01'; // Special non-printable character as prefix
		ft_strlcpy(final_token + 1, s, size + 1);
		free(s);
		add_token(tokens, final_token);
	}
	else
	{
		add_token(tokens, s);
	}
	
	t += size;
	if (*t == quote)
		t++;
	*temp = t;
	return ;
}

void	str_op(t_list **tokens, char **temp)
{
	int		i;
	int		x;
	char	*s;
	char	*t;

	i = 0;
	x = 0;
	t = *temp;
	if (*t == *(t + 1))
	{
		s = malloc(3);
		x = 1;
	}
	else
		s = malloc(2);
	if (!s)
		return ;
	s[i++] = *t++;
	if (x)
		s[i++] = *t++;
	s[i] = '\0';
	add_token(tokens, s);
	*temp = t;
}

void	str_word(t_list **tokens, char **temp)
{
	int		len;
	char	*s;
	char	*start;

	start = *temp;
	while (**temp && **temp != ' ' && **temp != '\'' && **temp != '\"' 
		&& **temp != '|' && **temp != '>' && **temp != '<')
		(*temp)++;
	len = *temp - start;
	s = malloc(len + 1);
	if (!s)
		return ;
	ft_strlcpy(s, start, len + 1);
	add_token(tokens, s);
}

char	**list_to_array(t_list *tokens, t_shell *shell)
{
	int		count;
	char	**args;
	t_list	*current;
	int		i;

	if (!tokens)
		return (NULL);
	count = 0;
	current = tokens;
	while (current)
	{
		count++;
		current = current->next;
	}
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	current = tokens;
	i = 0;
	while (current && i < count)
	{
		args[i] = expand_variables((char *)current->content, shell);
		current = current->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

// Expand environment variables in a string
char	*expand_variables(char *str, t_shell *shell)
{
	char	*result;
	char	*temp;
	char	*var_start;
	char	*var_end;
	char	*var_name;
	char	*var_value;
	int		len;

	if (!str)
		return (ft_strdup(""));
	
	// Check if this token was single-quoted (has special prefix)
	if (str[0] == '\x01')
	{
		// Return the string without the prefix (no expansion)
		return (ft_strdup(str + 1));
	}
	
	// No $ sign means no variables to expand
	if (!ft_strchr(str, '$'))
		return (ft_strdup(str));
	
	result = ft_strdup("");
	temp = str;
	
	while (*temp)
	{
		if (*temp == '$' && *(temp + 1))
		{
			if (*(temp + 1) == '?')
			{
				// Handle $? special variable
				char *exit_str = ft_itoa(shell->last_exit_status);
				char *new_result = ft_strjoin(result, exit_str);
				free(result);
				free(exit_str);
				result = new_result;
				temp += 2; // Skip $ and ?
			}
			else if (*(temp + 1) == '{')
			{
				// Handle ${VAR} syntax
				var_start = temp + 2; // Skip $ and {
				var_end = var_start;
				while (*var_end && *var_end != '}')
					var_end++;
				
				if (*var_end == '}' && var_end > var_start)
				{
					// Valid ${VAR} syntax
					len = var_end - var_start;
					var_name = malloc(len + 1);
					if (!var_name)
						return (result);
					ft_strlcpy(var_name, var_start, len + 1);
					
					var_value = getenv(var_name);
					if (var_value)
					{
						char *new_result = ft_strjoin(result, var_value);
						free(result);
						result = new_result;
					}
					free(var_name);
					temp = var_end + 1; // Skip the closing }
				}
				else
				{
					// Invalid ${VAR} syntax, treat as literal
					char char_str[2] = {*temp, '\0'};
					char *new_result = ft_strjoin(result, char_str);
					free(result);
					result = new_result;
					temp++;
				}
			}
			else if (ft_isdigit(*(temp + 1)))
			{
				// Handle positional parameters $1, $2, etc.
				var_start = temp + 1;
				var_end = var_start;
				while (*var_end && ft_isdigit(*var_end))
					var_end++;
				
				// For now, positional parameters are not implemented
				// They expand to empty string, continue with remaining text
				temp = var_end;
			}
			else if (ft_isalpha(*(temp + 1)) || *(temp + 1) == '_')
			{
				// Handle environment variables (must start with letter or underscore)
				var_start = temp + 1;
				var_end = var_start;
				while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
					var_end++;
				
				len = var_end - var_start;
				var_name = malloc(len + 1);
				if (!var_name)
					return (result);
				ft_strlcpy(var_name, var_start, len + 1);
				
				var_value = getenv(var_name);
				if (var_value)
				{
					char *new_result = ft_strjoin(result, var_value);
					free(result);
					result = new_result;
				}
				free(var_name);
				temp = var_end;
			}
			else
			{
				// $ followed by non-alphanumeric, treat as literal
				char char_str[2] = {*temp, '\0'};
				char *new_result = ft_strjoin(result, char_str);
				free(result);
				result = new_result;
				temp++;
			}
		}
		else
		{
			char char_str[2] = {*temp, '\0'};
			char *new_result = ft_strjoin(result, char_str);
			free(result);
			result = new_result;
			temp++;
		}
	}
	return (result);
}
