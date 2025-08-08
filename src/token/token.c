/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:06:03 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/08 18:09:50 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

void	free_tokens(t_list *tokens)
{
	t_list	*temp;
	t_token	*token;

	while (tokens)
	{
		temp = tokens->next;
		token = (t_token *)tokens->content;
		if (token)
		{
			free(token->value);
			free(token);
		}
		free(tokens);
		tokens = temp;
	}
}

static char	*str_extract(char **temp)
{
	int		size;
	char	quote;
	char	*s;
	char	*t;

	t = *temp;
	quote = *t++;
	size = token_size(t, quote);
	s = malloc(size + 1);
	if (!s)
		return (NULL);
	ft_memcpy(s, t, size);
	s[size] = '\0';
	t += size;
	if (*t == quote)
		t++;
	*temp = t;
	return (s);
}

static void	str_combined_word(t_list **tokens, char **temp)
{
	char	*combined;
	char	*part;
	t_token	*token;
	char	quote_type = 0;

	combined = ft_strdup("");
	if (!combined)
		return ;
	while (**temp && **temp != ' ' && **temp != '\t' && **temp != '|' 
		&& **temp != '>' && **temp != '<')
	{
		if (**temp == '\'' || **temp == '\"')
		{
			if (!quote_type)
				quote_type = **temp;  // Store first quote type encountered
			part = str_extract(temp);
			if (!part)
			{
				free(combined);
				return ;
			}
		}
		else
		{
			char *word_start = *temp;
			while (**temp && **temp != ' ' && **temp != '\t' && **temp != '\''
				&& **temp != '\"' && **temp != '|' && **temp != '>'
				&& **temp != '<')
				(*temp)++;
			int len = *temp - word_start;
			part = malloc(len + 1);
			if (!part)
			{
				free(combined);
				return ;
			}
			ft_strlcpy(part, word_start, len + 1);
		}
		char *new_combined = ft_strjoin(combined, part);
		free(combined);
		free(part);
		if (!new_combined)
			return ;
		combined = new_combined;
	}
	token = malloc(sizeof(t_token));
	if (!token)
	{
		free(combined);
		return ;
	}
	token->value = combined;
	token->type = WORD;
	token->quote_type = quote_type;
	add_token(tokens, token);
}

t_list	*tokenize(const char *input)
{
	int		i;
	char	*temp;
	t_list	*tokens;

	i = 0;
	tokens = NULL;
	if (!input || !*input)
		return (NULL);
	temp = (char *)input;
	while (*temp)
	{
		while (*temp == ' ' || *temp == '\t')
			temp++;
		if (!*temp)
			break ;
		if (*temp == '|' || *temp == '>' || *temp == '<')
			str_op(&tokens, &temp);
		else
			str_combined_word(&tokens, &temp);
	}
	return (tokens);
}
