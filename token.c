/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:06:03 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/01 15:41:01 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

void	add_token(t_list **tokens, char *s)
{
	t_list	*new_node;

	new_node = ft_lstnew(temp);
	if (!new_node)
		return ;
	ft_lstadd_back(&tokens, new_node);
}

int	token_size(char *s, char quote)
{
	int	size;

	size = 0;
	while (s[size] && s[size] != quote)
		size++;
	return (size);
}

t_list	*tokenize(const char *input)
{
	int	i;
	int	size;
	char	quote;
	char	*s;
	char	*temp;
	t_list	*tokens;

	i = 0;
	size = 0;
	tokens = NULL;
	temp = (char *)input;
	while(*temp)
	{
		while (*temp == ' ')
			temp++;
		if (*temp == '\'' || *temp == '\"' )
		{
			quote = *temp++;
			size = token_size(temp, quote);
			s = malloc(size + 1);
			if (!s)
				return (NULL);
			while (i < size)
				s[i++] = temp[i++];
			s = '\0';
			add_token(&tokens, s);
			temp += size;
			if (*temp == quote)
				temp++;
		}
		if (*temp == '|' || *temp == '>' || *temp =='<')
		{
			s = input[i++];
			if (input[i - 1] == input[i])
				tokens[j++] = input[i++];

	}
}
