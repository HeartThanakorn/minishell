/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:44:55 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/07 18:42:31 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

void	add_token(t_list **tokens, t_token *token)
{
	t_list	*new_node;

	new_node = ft_lstnew(token);
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

static char	*op_extract(char **temp)
{
	int		i;
	int		is_double;
	char	*s;
	char	*t;

	i = 0;
	is_double = 0;
	t = *temp;
	if (*t == *(t + 1))
	{
		s = malloc(3);
		is_double = 1;
	}
	else
		s = malloc(2);
	if (!s)
		return (NULL);
	s[i++] = *t++;
	if (is_double)
		s[i++] = *t++;
	s[i] = '\0';
	*temp = t;
	return (s);
}

void	str_op(t_list **tokens, char **temp)
{
	t_token	*token;
	char	*s;

	s = op_extract(temp);
	if (!s)
		return ;
	token = malloc(sizeof(t_token));
	if (!token)
		return (free(s), (void)0);
	token->value = s;
	if (ft_strncmp(s, ">>", 3) == 0)
		token->type = REDIR_APPEND;
	else if (ft_strncmp(s, "<<", 3) == 0)
		token->type = REDIR_HERE_DOC;
	else if (ft_strncmp(s, "<", 2) == 0)
		token->type = REDIR_IN;
	else if (ft_strncmp(s, ">", 2) == 0)
		token->type = REDIR_OUT;
	else if (ft_strncmp(s, "|", 2) == 0)
		token->type = PIPE;
	else
		token->type = WORD;
	token->quote_type = 0;
	add_token(tokens, token);
}

void	str_word(t_list **tokens, char **temp)
{
	int		len;
	char	*s;
	char	*start;
	t_token	*token;

	start = *temp;
	while (**temp && **temp != ' ' && **temp != '\t' && **temp != '\''
		&& **temp != '\"' && **temp != '|' && **temp != '>'
		&& **temp != '<')
		(*temp)++;
	len = *temp - start;
	s = malloc(len + 1);
	if (!s)
		return ;
	token = malloc(sizeof(t_token));
	if (!token)
	{
		free(s);
		return ;
	}
	ft_strlcpy(s, start, len + 1);
	token->value = s;
	token->type = WORD;
	token->quote_type = 0;
	add_token(tokens, token);
}
