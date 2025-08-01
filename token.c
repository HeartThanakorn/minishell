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

	while (tokens)
	{
		temp = tokens->next;
		free(tokens->content);
		free(tokens);
		tokens = temp;
	}
}

t_list	*tokenize(const char *input)
{
	int		i;
	char	*s;
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
