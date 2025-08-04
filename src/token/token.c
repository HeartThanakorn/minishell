/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:06:03 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/04 17:39:57 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

void	free_tokens(t_list *tokens)
{
	t_list	*temp;

	while (tokens)
	{
		temp = tokens->next;
		free(tokens->content);
		free(tokens);
		tokens = temp;
	}
}

// Check if a token is an operator
int	is_operator(char *token)
{
	if (!token)
		return (0);
	if ((ft_strncmp(token, "|", 2) == 0 && ft_strlen(token) == 1) ||
		(ft_strncmp(token, ">", 2) == 0 && ft_strlen(token) == 1) ||
		(ft_strncmp(token, "<", 2) == 0 && ft_strlen(token) == 1) ||
		(ft_strncmp(token, ">>", 3) == 0 && ft_strlen(token) == 2) ||
		(ft_strncmp(token, "<<", 3) == 0 && ft_strlen(token) == 2))
		return (1);
	return (0);
}

// Build a complete token by concatenating adjacent parts (words and quotes)
char	*build_complete_token(char **temp)
{
	char	*result;
	char	*part;
	char	*new_result;
	char	*start_pos;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	
	start_pos = *temp;
	
	// Handle operators first (they are always separate tokens)
	if (**temp == '|' || **temp == '>' || **temp == '<')
	{
		free(result);
		return (extract_operator(temp));
	}
	
	// Build concatenated token from words and quotes
	while (**temp && **temp != ' ')
	{
		if (**temp == '\'' || **temp == '\"')
		{
			part = extract_quoted_part(temp);
			if (!part)  // Unclosed quote error
			{
				free(result);
				return (NULL);
			}
		}
		else if (**temp == '|' || **temp == '>' || **temp == '<')
			break; // Stop at operators
		else
			part = extract_word_part(temp);
		
		if (!part)
		{
			free(result);
			return (NULL);
		}
		
		new_result = ft_strjoin(result, part);
		free(result);
		free(part);
		if (!new_result)
			return (NULL);
		result = new_result;
	}
	
	return (result);
}

// Extract operator token
char	*extract_operator(char **temp)
{
	char	*result;
	char	*start;
	int	len;

	start = *temp;
	if (**temp == *(*temp + 1)) // >> or <<
	{
		len = 2;
		*temp += 2;
	}
	else
	{
		len = 1;
		(*temp)++;
	}
	
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len + 1);
	return (result);
}

// Extract word part (up to quote, operator, or space)
char	*extract_word_part(char **temp)
{
	char	*start;
	char	*result;
	int	len;

	start = *temp;
	while (**temp && **temp != ' ' && **temp != '\'' && **temp != '\"'
		&& **temp != '|' && **temp != '>' && **temp != '<')
		(*temp)++;
	
	len = *temp - start;
	if (len == 0)
		return (ft_strdup(""));
	
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len + 1);
	return (result);
}

// Extract quoted part and return the content with quote type prefix
char	*extract_quoted_part(char **temp)
{
	char	quote_char;
	char	*start;
	char	*result;
	char	*final_result;
	int	len;

	quote_char = **temp;
	(*temp)++; // Skip opening quote
	start = *temp;
	
	// Find closing quote
	while (**temp && **temp != quote_char)
		(*temp)++;
	
	// Check for unclosed quote
	if (**temp != quote_char)
	{
		// Unclosed quote - return NULL to indicate error
		return (NULL);
	}
	
	len = *temp - start;
	(*temp)++; // Skip closing quote
	
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len + 1);
	
	// Add prefix to indicate quote type for variable expansion
	if (quote_char == '\'')
	{
		// Single quotes: add \x01 prefix to prevent variable expansion
		final_result = malloc(len + 2);
		if (!final_result)
		{
			free(result);
			return (NULL);
		}
		final_result[0] = '\x01';
		ft_strlcpy(final_result + 1, result, len + 1);
		free(result);
		return (final_result);
	}
	else
	{
		// Double quotes: no prefix, allow variable expansion
		return (result);
	}
}

t_list	*tokenize(const char *input)
{
	char	*temp;
	t_list	*tokens;
	char	*concatenated_token;

	tokens = NULL;
	if (!input || !*input)
		return (NULL);
	temp = (char *)input;
	while (*temp)
	{
		while (*temp == ' ')
			temp++;
		if (*temp == '\0')
			break;
		
		// Build a complete token by concatenating adjacent parts
		concatenated_token = build_complete_token(&temp);
		if (!concatenated_token)
		{
			// Error occurred (likely unclosed quote)
			free_tokens(tokens);
			return (NULL);
		}
		if (is_operator(concatenated_token))
			add_token(&tokens, concatenated_token);
		else
			add_token(&tokens, concatenated_token);
	}
	return (tokens);
}
