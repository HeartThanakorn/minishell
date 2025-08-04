/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:06:32 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/01 14:16:30 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

typedef enum
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HERE_DOC
}	TokenType;

typedef struct s_token
{
	char	*value;
	TokenType	type;
} t_token;

t_list	*tokenize(const char *input);
void	free_tokens(t_list *token_list);
char	**list_to_array(t_list *tokens);

#endif
