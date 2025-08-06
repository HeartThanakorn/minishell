/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:06:32 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/06 12:39:02 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "libft.h"

typedef enum e_tok
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HERE_DOC
}	t_TokenType;

typedef enum e_quote
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_QuoteType;

typedef struct s_token
{
	t_TokenType	type;
	t_QuoteType	quote_type;
	char		*value;
}	t_token;

t_list	*tokenize(const char *input);
void	free_tokens(t_list *token_list);
char	**list_to_array(t_list *tokens);
void	str_word(t_list **tokens, char **temp);
void	str_op(t_list **tokens, char **temp);
int		token_size(char *s, char quote);
void	add_token(t_list **tokens, t_token *token);

#endif
