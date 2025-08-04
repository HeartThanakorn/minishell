/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tthajan <tthajan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:06:32 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/04 18:27:11 by tthajan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "../libft/libft.h"

// Forward declaration
typedef struct s_shell t_shell;

t_list	*tokenize(const char *input);
void	free_tokens(t_list *token_list);
char	**list_to_array(t_list *tokens, t_shell *shell);
char	*expand_variables(char *str, t_shell *shell);

// Helper functions from token_utils.c
void	str_quote(t_list **tokens, char **temp);
void	str_op(t_list **tokens, char **temp);
void	str_word(t_list **tokens, char **temp);
void	add_token(t_list **tokens, char *s);
int	token_size(char *s, char quote);

// New tokenization functions from token.c
int	is_operator(char *token);
char	*build_complete_token(char **temp);
char	*extract_operator(char **temp);
char	*extract_word_part(char **temp);
char	*extract_quoted_part(char **temp);

#endif
