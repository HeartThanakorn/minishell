/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:06:03 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/01 17:57:12 by kmaeda           ###   ########.fr       */
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

static void	str_quote(t_list **tokens, char **temp)
{
	t_token	*token;
	char	*s;

	s = str_extract(temp);
	if (!s)
		return ;
	token = malloc(sizeof(t_token));
	if (!token)
	{
		free(s);
		return ;
	}
	token->value = s;
	token->type = WORD;
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
		while (*temp == ' ')
			temp++;
		if (*temp == '\'' || *temp == '\"' )
			str_quote(&tokens, &temp);
		else if (*temp == '|' || *temp == '>' || *temp == '<')
			str_op(&tokens, &temp);
		else
			str_word(&tokens, &temp);
	}
	return (tokens);
}
