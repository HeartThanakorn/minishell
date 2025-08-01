/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:44:55 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/01 17:49:12 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

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
	add_token(tokens, s);
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
	char	*t;
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
